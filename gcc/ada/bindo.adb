------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                                B I N D O                                 --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--             Copyright (C) 2019-2025, Free Software Foundation, Inc.      --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNAT; see file COPYING3.  If not, go to --
-- http://www.gnu.org/licenses for a complete copy of the license.          --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

with Binde;
with Opt;   use Opt;

with Bindo.Elaborators;
use  Bindo.Elaborators;

package body Bindo is

   ---------------------------------
   -- Elaboration-order mechanism --
   ---------------------------------

   --  The elaboration-order (EO) mechanism implemented in this unit and its
   --  children has the following objectives:
   --
   --    * Find an ordering of all library items (historically referred to as
   --      "units") in the bind which require elaboration, taking into account:
   --
   --        - The dependencies between units expressed in the form of with
   --          clauses.
   --
   --        - Pragmas Elaborate, Elaborate_All, Elaborate_Body, Preelaborable,
   --          and Pure.
   --
   --        - The flow of execution at elaboration time.
   --
   --        - Additional dependencies between units supplied to the binder by
   --          means of a forced-elaboration-order file.
   --
   --      The high-level idea empoyed by the EO mechanism is to construct two
   --      graphs and use the information they represent to find an ordering of
   --      all units.
   --
   --      The invocation graph represents the flow of execution at elaboration
   --      time.
   --
   --      The library graph captures the dependencies between units expressed
   --      by with clause and elaboration-related pragmas. The library graph is
   --      further augmented with additional information from the invocation
   --      graph by exploring the execution paths from a unit with elaboration
   --      code to other external units.
   --
   --      The strongly connected components of the library graph are computed.
   --
   --      The order is obtained using a topological sort-like algorithm which
   --      traverses the library graph and its strongly connected components in
   --      an attempt to order available units while enabling other units to be
   --      ordered.
   --
   --    * Diagnose elaboration circularities between units
   --
   --      An elaboration circularity arises when either
   --
   --        - At least one unit cannot be ordered, or
   --
   --        - All units can be ordered, but an edge with an Elaborate_All
   --          pragma links two vertices within the same component of the
   --          library graph.
   --
   --      The library graph is traversed to discover, collect, and sort all
   --      cycles that hinder the elaboration order.
   --
   --      The most important cycle is diagnosed by describing its effects on
   --      the elaboration order and listing all units comprising the circuit.
   --      Various suggestions on how to break the cycle are offered.

   -----------------
   -- Terminology --
   -----------------

   --  * Component - A strongly connected component of a graph.
   --
   --  * Elaborable component - A component that is not waiting on other
   --    components to be elaborated.
   --
   --  * Elaborable vertex - A vertex that is not waiting on strong and weak
   --    predecessors, and whose component is elaborable.
   --
   --  * Elaboration circularity - A cycle involving units from the bind.
   --
   --  * Elaboration root - A special invocation construct which denotes the
   --    elaboration procedure of a unit.
   --
   --  * Invocation - The act of activating a task, calling a subprogram, or
   --    instantiating a generic.
   --
   --  * Invocation construct - An entry declaration, [single] protected type,
   --    subprogram declaration, subprogram instantiation, or a [single] task
   --    type declared in the visible, private, or body declarations of some
   --    unit. The construct is encoded in the ALI file of the related unit.
   --
   --  * Invocation graph - A directed graph which models the flow of execution
   --    at elaboration time.
   --
   --      - Vertices - Invocation constructs plus extra information. Certain
   --        vertices act as elaboration roots.
   --
   --      - Edges - Invocation relations plus extra information.
   --
   --  * Invocation relation - A flow link between two invocation constructs.
   --    This link is encoded in the ALI file of unit that houses the invoker.
   --
   --  * Invocation signature - A set of attributes that uniquely identify an
   --    invocation construct within the namespace of all ALI files.
   --
   --  * Invoker - The source construct of an invocation relation (the caller,
   --    instantiator, or task activator).
   --
   --  * Library graph - A directed graph which captures with clause and pragma
   --    dependencies between units.
   --
   --      - Vertices - Units plus extra information.
   --
   --      - Edges - With clause, pragma, and additional dependencies between
   --        units.
   --
   --  * Pending predecessor - A vertex that must be elaborated before another
   --    vertex can be elaborated.
   --
   --  * Strong edge - A non-invocation library graph edge. Strong edges
   --    represent the language-defined relations between units.
   --
   --  * Strong predecessor - A library graph vertex reachable via a strong
   --    edge.
   --
   --  * Target - The destination construct of an invocation relation (the
   --    generic, subprogram, or task type).
   --
   --  * Weak edge - An invocation library graph edge. Weak edges represent
   --    the speculative flow of execution at elaboration time, which may or
   --    may not take place.
   --
   --  * Weak predecessor - A library graph vertex reachable via a weak edge.
   --
   --  * Weakly elaborable vertex - A vertex that is waiting solely on weak
   --    predecessors to be elaborated, and whose component is elaborable.

   ------------------
   -- Architecture --
   ------------------

   --     Find_Elaboration_Order
   --     |
   --     +--> Collect_Elaborable_Units
   --     +--> Write_ALI_Tables
   --     +--> Elaborate_Units
   --          |
   --  +------ | -------------- Construction phase ------------------------+
   --  |       |                                                           |
   --  |       +--> Build_Library_Graph                                    |
   --  |       +--> Validate_Library_Graph                                 |
   --  |       +--> Write_Library_Graph                                    |
   --  |       |                                                           |
   --  |       +--> Build_Invocation_Graph                                 |
   --  |       +--> Validate_Invocation_Graph                              |
   --  |       +--> Write_Invocation_Graph                                 |
   --  |       |                                                           |
   --  +------ | ----------------------------------------------------------+
   --          |
   --  +------ | -------------- Augmentation phase ------------------------+
   --  |       |                                                           |
   --  |       +--> Augment_Library_Graph                                  |
   --  |       |                                                           |
   --  +------ | ----------------------------------------------------------+
   --          |
   --  +------ | -------------- Ordering phase ----------------------------+
   --  |       |                                                           |
   --  |       +--> Find_Components                                        |
   --  |       |                                                           |
   --  |       +--> Elaborate_Library_Graph                                |
   --  |       +--> Validate_Elaboration_Order                             |
   --  |       +--> Write_Elaboration_Order                                |
   --  |       |                                                           |
   --  |       +--> Write_Unit_Closure                                     |
   --  |       |                                                           |
   --  +------ | ----------------------------------------------------------+
   --          |
   --  +------ | -------------- Diagnostics phase -------------------------+
   --  |       |                                                           |
   --  |       +--> Find_Cycles                                            |
   --  |       +--> Validate_Cycles                                        |
   --  |       +--> Write_Cycles                                           |
   --  |       |                                                           |
   --  |       +--> Diagnose_Cycle / Diagnose_All_Cycles                   |
   --  |                                                                   |
   --  +-------------------------------------------------------------------+

   ------------------------
   -- Construction phase --
   ------------------------

   --  The Construction phase has the following objectives:
   --
   --    * Build the library graph by inspecting the ALI file of each unit that
   --      requires elaboration.
   --
   --    * Validate the consistency of the library graph, only when switch -d_V
   --      is in effect.
   --
   --    * Write the contents of the invocation graph in human-readable form to
   --      standard output when switch -d_L is in effect.
   --
   --    * Build the invocation graph by inspecting invocation constructs and
   --      relations in the ALI file of each unit that requires elaboration.
   --
   --    * Validate the consistency of the invocation graph, only when switch
   --      -d_V is in effect.
   --
   --    * Write the contents of the invocation graph in human-readable form to
   --      standard output when switch -d_I is in effect.

   ------------------------
   -- Augmentation phase --
   ------------------------

   --  The Augmentation phase has the following objectives:
   --
   --    * Discover transitions of the elaboration flow from a unit with an
   --      elaboration root to other units. Augment the library graph with
   --      extra edges for each such transition.

   --------------------
   -- Ordering phase --
   --------------------

   --  The Ordering phase has the following objectives:
   --
   --    * Discover all components of the library graph by treating specs and
   --      bodies as single vertices.
   --
   --    * Try to order as many vertices of the library graph as possible by
   --      performing a topological sort based on the pending predecessors of
   --      vertices across all components and within a single component.
   --
   --    * Validate the consistency of the order, only when switch -d_V is in
   --      effect.
   --
   --    * Write the contents of the order in human-readable form to standard
   --      output when switch -d_O is in effect.
   --
   --    * Write the sources of the order closure when switch -R is in effect.

   -----------------------
   -- Diagnostics phase --
   -----------------------

   --  The Diagnostics phase has the following objectives:
   --
   --    * Discover, save, and sort all cycles in the library graph. The cycles
   --      are sorted based on the following heuristics:
   --
   --        - A cycle with higher precedence is preferred.
   --
   --        - A cycle with fewer invocation edges is preferred.
   --
   --        - A cycle with a shorter length is preferred.
   --
   --    * Validate the consistency of cycles, only when switch -d_V is in
   --      effect.
   --
   --    * Write the contents of all cycles in human-readable form to standard
   --      output when switch -d_O is in effect.
   --
   --    * Diagnose the most important cycle, or all cycles when switch -d_C is
   --      in effect. The diagnostic consists of:
   --
   --        - The reason for the existence of the cycle, along with the unit
   --          whose elaboration cannot be guaranteed.
   --
   --        - A detailed traceback of the cycle, showcasing the transition
   --          between units, along with any other elaboration-order-related
   --          information.
   --
   --        - A set of suggestions on how to break the cycle considering the
   --          the edges comprising the circuit, the elaboration model used to
   --          compile the units, the availability of invocation information,
   --          and the state of various relevant switches.

   --------------
   -- Switches --
   --------------

   --  -d_a  Ignore the effects of pragma Elaborate_All
   --
   --        GNATbind creates a regular with edge instead of an Elaborate_All
   --        edge in the library graph, thus eliminating the effects of the
   --        pragma.
   --
   --  -d_b  Ignore the effects of pragma Elaborate_Body
   --
   --        GNATbind treats a spec and body pair as decoupled.
   --
   --  -d_e  Ignore the effects of pragma Elaborate
   --
   --        GNATbind creates a regular with edge instead of an Elaborate edge
   --        in the library graph, thus eliminating the effects of the pragma.
   --        In addition, GNATbind does not create an edge to the body of the
   --        pragma argument.
   --
   --  -d_t  Output cycle-detection trace information
   --
   --        GNATbind outputs trace information on cycle-detection activities
   --        to standard output.
   --
   --  -d_A  Output ALI invocation tables
   --
   --        GNATbind outputs the contents of ALI table Invocation_Constructs
   --        and Invocation_Edges in textual format to standard output.
   --
   --  -d_C  Diagnose all cycles
   --
   --        GNATbind outputs diagnostics for all unique cycles in the bind,
   --        rather than just the most important one.
   --
   --  -d_I  Output invocation graph
   --
   --        GNATbind outputs the invocation graph in text format to standard
   --        output.
   --
   --  -d_L  Output library graph
   --
   --        GNATbind outputs the library graph in textual format to standard
   --        output.
   --
   --  -d_P  Output cycle paths
   --
   --        GNATbind outputs the cycle paths in text format to standard output
   --
   --  -d_S  Output elaboration-order status information
   --
   --        GNATbind outputs trace information concerning the status of its
   --        various phases to standard output.
   --
   --  -d_T  Output elaboration-order trace information
   --
   --        GNATbind outputs trace information on elaboration-order detection
   --        activities to standard output.
   --
   --  -d_V  Validate bindo cycles, graphs, and order
   --
   --        GNATbind validates the invocation graph, library graph along with
   --        its cycles, and elaboration order by detecting inconsistencies and
   --        producing error reports.
   --
   --  -e    Output complete list of elaboration-order dependencies
   --
   --        GNATbind outputs the dependencies between units to standard
   --        output.
   --
   --  -f    Force elaboration order from given file
   --
   --        GNATbind applies an additional set of edges to the library graph.
   --        The edges are read from a file specified by the argument of the
   --        flag.
   --
   --  -H    Legacy elaboration-order model enabled
   --
   --        GNATbind uses the library-graph and heuristics-based elaboration-
   --        order model.
   --
   --  -l    Output chosen elaboration order
   --
   --        GNATbind outputs the elaboration order in text format to standard
   --        output.
   --
   --  -p    Pessimistic (worst-case) elaboration order
   --
   --        This switch is not used in Bindo and its children.

   ----------------------------------------
   -- Debugging elaboration-order issues --
   ----------------------------------------

   --  Prior to debugging elaboration-order-related issues, enable all relevant
   --  debug flags to collect as much information as possible. Depending on the
   --  number of files in the bind, Bindo may emit anywhere between several MBs
   --  to several hundred MBs of data to standard output. The switches are:
   --
   --    -d_A -d_C -d_I -d_L -d_P -d_t -d_T -d_V
   --
   --  Bindo offers several debugging routines that can be invoked from gdb.
   --  Those are defined in the body of Bindo.Writers, in sections denoted by
   --  header Debug. For quick reference, the routines are:
   --
   --    palgc  --  print all library-graph cycles
   --    pau    --  print all units
   --    pc     --  print component
   --    pige   --  print invocation-graph edge
   --    pigv   --  print invocation-graph vertex
   --    plgc   --  print library-graph cycle
   --    plge   --  print library-graph edge
   --    plgv   --  print library-graph vertex
   --    pu     --  print units
   --
   --  * Apparent infinite loop
   --
   --    The elaboration order mechanism appears to be stuck in an infinite
   --    loop. Use switch -d_S to output the status of each elaboration phase.
   --
   --  * Invalid elaboration order
   --
   --    The elaboration order is invalid when:
   --
   --      - A unit that requires elaboration is missing from the order
   --      - A unit that does not require elaboration is present in the order
   --
   --    Examine the output of the elaboration algorithm available via switch
   --    -d_T to determine how the related units were included in or excluded
   --    from the order. Determine whether the library graph contains all the
   --    relevant edges for those units.
   --
   --    Units and routines of interest:
   --      Bindo.Elaborators
   --      Elaborate_Library_Graph
   --      Elaborate_Units
   --
   --  * Invalid invocation graph
   --
   --    The invocation graph is invalid when:
   --
   --      - An edge lacks an attribute
   --      - A vertex lacks an attribute
   --
   --    Find the malformed edge or vertex and determine which attribute is
   --    missing. Examine the contents of the invocation-related ALI tables
   --    available via switch -d_A. If the invocation construct or relation
   --    is missing, verify the ALI file. If the ALI lacks all the relevant
   --    information, then Sem_Elab most likely failed to discover a valid
   --    elaboration path.
   --
   --    Units and routines of interest:
   --      Bindo.Builders
   --      Bindo.Graphs
   --      Add_Edge
   --      Add_Vertex
   --      Build_Invocation_Graph
   --
   --  * Invalid library graph
   --
   --    The library graph is invalid when:
   --
   --      - An edge lacks an attribute
   --      - A vertex lacks an attribute
   --
   --    Find the malformed edge or vertex and determine which attribute is
   --    missing.
   --
   --    Units and routines of interest:
   --      Bindo.Builders
   --      Bindo.Graphs
   --      Add_Edge
   --      Add_Vertex
   --      Build_Library_Graph
   --
   --  * Invalid library-graph cycle
   --
   --    A library-graph cycle is invalid when:
   --
   --      - It lacks enough edges to form a circuit
   --      - At least one edge in the circuit is repeated
   --
   --    Find the malformed cycle and determine which attribute is missing.
   --
   --    Units and routines of interest:
   --      Bindo.Graphs
   --      Find_Cycles

   ----------------------------
   -- Find_Elaboration_Order --
   ----------------------------

   procedure Find_Elaboration_Order
     (Order         : out Unit_Id_Table;
      Main_Lib_File : File_Name_Type)
   is
   begin
      --  Use the library graph and heuristic-based elaboration order when
      --  switch -H (legacy elaboration-order mode enabled).

      if Legacy_Elaboration_Order then
         Binde.Find_Elab_Order (Order, Main_Lib_File);

      --  Otherwise use the invocation and library-graph-based elaboration
      --  order.

      else
         Invocation_And_Library_Graph_Elaborators.Elaborate_Units
           (Order         => Order,
            Main_Lib_File => Main_Lib_File);
      end if;
   end Find_Elaboration_Order;

end Bindo;
