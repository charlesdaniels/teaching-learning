/* Simple demonstration application -- implements a small graph editor using
 * LibAgar's graph widget. */

/*
 * Copyright (c) 2019, Charles Daniels All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <agar/core.h>
#include <agar/gui.h>

/* We use a global state variable to handle creating edges so we know weather
 * we are currently selecting the first edge, or the second edge. This
 * should probably be an enum. */

#define state_ready 0  /* not currently creating an edge */
#define state_v1 1     /* selecting the first vertex */
#define state_v2 2     /* selecting the second vertex */

/* handler for "quit" menu item */
void QuitApplication(AG_Event *event) {
	AG_Quit();
}

/* If the debugger is enabled, the "Launch Debugger" menu item will use this
 * to display the LibAgar debugging window */
#ifdef ENABLE_DEBUGGER
void LaunchDebugger(AG_Event *event) {
	AG_Driver* dri;
	AG_Window* db_window;

	/* Fetch the top-level driver object, which is also the parent of the
	 * main window. We will attach the debugger window to this object. */
	dri = (AG_Driver*) AG_ObjectRoot(AG_SELF());

	db_window = AG_GuiDebugger(dri);
	AG_WindowShow(db_window);
}
#endif

/* Add a new vertex */
void AddVertex(AG_Event *event) {

	/* AG uses an object tree, rooted at the driver object. We can call
	 * AG_Object_Root to traverse all the way up the tree to the driver, so
	 * we can safely use that as a convenient place to stick variables */
	AG_Driver* dri = (AG_Driver*) AG_ObjectRoot(AG_SELF());

	/* We have a raw pointer to the graph widget stored by main() during
	 * initialization */
	AG_Graph* g = (AG_Graph*) AG_GetPointer(dri, "graph_p");
	AG_Label* statuslabel = (AG_Label*) AG_GetPointer(dri, "status_label_p");

	/* Create the new vertex. We assign it an empty label due to LibAgar
	 * issue #13 (https://github.com/JulNadeauCA/libagar/issues/13) */
	AG_GraphVertex* v = AG_GraphVertexNew(g, NULL);
	AG_GraphVertexLabelS(v, "");

	AG_LabelText(statuslabel, "vertex created... ready.");
}

/* Automatically layout all of the nodes we have already placed to try and
 * ensure no overlapping edges */
void AutoLayout(AG_Event* event) {
	AG_Driver* dri = (AG_Driver*) AG_ObjectRoot(AG_SELF());
	AG_Graph* g = (AG_Graph*) AG_GetPointer(dri, "graph_p");
	AG_Label* statuslabel = (AG_Label*) AG_GetPointer(dri, "status_label_p");

	/* Somewhat naughty -- should probably query the graph object
	 * to find it's size */
	AG_GraphAutoPlace(g, 800, 600);

	AG_LabelText(statuslabel, "auto layout done... ready.");
}


/* Create a new edge. This is done by putting us in state_v1, which 
 * HandleVertexSelection will use when we click on a vertex to know that it
 * should populate the relevant variable */
void AddEdge(AG_Event* event) {
	AG_Driver* dri = (AG_Driver*) AG_ObjectRoot(AG_SELF());

	/* This is much like the grap_p object -- status_label_p is set
	 * during initialization to be the pointer to the status label. */
	AG_Label* statuslabel = (AG_Label*) AG_GetPointer(dri, "status_label_p");

	/* Tell the user it's time to select a vertex */
	AG_LabelText(statuslabel, "creating edge: select first vertex");

	/* Set the state */
	AG_SetUint(dri, "state", state_v1);
}


/* This event handler fires whenever the user clicks a vertex. If we are in the
 * process of creating an edge, then this function handles the edge selection
 * logic, and also creates the edge when ready */
void HandleVertexSelection(AG_Event* event) {
	/* The vertex which was selected is passed to the graph-vertex-selected
	 * handler. If we are in state_v1 or _v2, we will save this to
	 * a variable for use when creating an edge later */
	AG_GraphVertex* vtx = (AG_GraphVertex*) AG_PTR(1);

	AG_Driver* dri = (AG_Driver*) AG_ObjectRoot(AG_SELF());
	AG_Label* statuslabel = (AG_Label*) AG_GetPointer(dri, "status_label_p");
	AG_Graph* g = (AG_Graph*) AG_GetPointer(dri, "graph_p");

	/* we are currently selecting the first vertex */
	if (AG_GetUint(dri, "state") == state_v1) {
		
		/* set the variable for the selected vertex */
		AG_SetPointer(dri, "selected_vertex", vtx);

		/* update the state -- the next vertex we click will be the
		 * second */
		AG_SetUint(dri, "state", state_v2);

		/* tell the userit's time to select the next vertex */
		AG_LabelText(statuslabel, "creating edge: select second vertex");

	/* we are currently selecting the second vertex */
	} else if (AG_GetUint(dri, "state") == state_v2) {

		/* regardless of weather or not the new edge is valid, we are
		 * ready to accept a new vertex selection */
		AG_SetUint(dri, "state", state_ready);

		if (AG_GetPointer(dri, "selected_vertex") == vtx) {
			/* don't allow a vertex to be linked to itself */
			AG_LabelText(statuslabel, "ERROR: source and destination vertices must be different... ready.");

		} else {
			/* create the edge */
			AG_GraphEdgeNew(g, AG_GetPointer(dri, "selected_vertex"), vtx, NULL);
			AG_LabelText(statuslabel, "edge created... ready.");
		}
	}

}

void CreateSomeNodes(AG_Event* event) {
	/* Create some nodes and then lay them out using the auto-layout. This
	 * is just to get some vertices and edges out for demo purposes. */

	AG_Driver* dri = (AG_Driver*) AG_ObjectRoot(AG_SELF());
	AG_Label* statuslabel = (AG_Label*) AG_GetPointer(dri, "status_label_p");
	AG_Graph* g = (AG_Graph*) AG_GetPointer(dri, "graph_p");

	/* instantiate the vertices */
	AG_GraphVertex* v1 = AG_GraphVertexNew(g, NULL);
	AG_GraphVertexLabelS(v1, "1");
	AG_GraphVertex* v2 = AG_GraphVertexNew(g, NULL);
	AG_GraphVertexLabelS(v2, "2");
	AG_GraphVertex* v3 = AG_GraphVertexNew(g, NULL);
	AG_GraphVertexLabelS(v3, "3");
	AG_GraphVertex* v4 = AG_GraphVertexNew(g, NULL);
	AG_GraphVertexLabelS(v4, "4");
	AG_GraphVertex* v5 = AG_GraphVertexNew(g, NULL);
	AG_GraphVertexLabelS(v5, "5");

	/* instantiate some edges */
	AG_GraphEdgeNew(g, v1, v2, NULL);
	AG_GraphEdgeNew(g, v2, v2, NULL);
	AG_GraphEdgeNew(g, v3, v2, NULL);
	AG_GraphEdgeNew(g, v2, v4, NULL);
	AG_GraphEdgeNew(g, v2, v5, NULL);
	AG_GraphEdgeNew(g, v1, v5, NULL);
	AG_GraphEdgeNew(g, v3, v4, NULL);

	/* lay them out automatically */
	AG_GraphAutoPlace(g, 800, 600);

	AG_LabelText(statuslabel, "created some nodes for you... ready.");
}

int main(int argc, char *argv[]) {
	AG_Window *win;
	AG_Graph* g;
	AG_Statusbar* statusbar;
	AG_Label* statuslabel;
	AG_Driver* dri;
	AG_MenuItem* temp;
	AG_Menu* menu;

	/* Initialize LibAgar */
	if (AG_InitCore(NULL, 0) == -1 ||
			AG_InitGraphics(0) == -1)
		return (1);
	win = AG_WindowNew(0);

	/* set the window title */
	AG_WindowSetCaptionS (win, "Graph Editor");

	/* setup the state handler and edge creation vertex variables */
	dri = AG_ObjectRoot(win);
	AG_SetUint(dri, "state", state_ready);
	AG_SetPointer(dri, "selected_vertex", NULL);

	menu = AG_MenuNew(win, 0);

	/* instantiate the graph */
	g = AG_GraphNew(win, AG_GRAPH_EXPAND);
	AG_GraphSizeHint(g, 800, 600);

	/* register the relevant handler to the graph-vertex-selected event */
	AG_AddEvent(g, "graph-vertex-selected", HandleVertexSelection, NULL);

	/* We will use this variable later so that we can access the graph
	 * widget from our event handlers. There is probably a better way to
	 * do this; this approach is a bit kludged since it subverts the
	 * type system */
	AG_SetPointer(dri, "graph_p", g);

	/* instantiate the "File" menu dropdown */
	AG_MenuItem* menu_file = AG_MenuNode(menu->root, "File", NULL);

	/* instantiate the contents of the File menu */
	{

		AG_MenuAction(menu_file, "Add Vertex", NULL, AddVertex, NULL);
		AG_MenuAction(menu_file, "Auto Layout", NULL, AutoLayout, NULL);
		AG_MenuAction(menu_file, "Demo", NULL, CreateSomeNodes, NULL);
		AG_MenuAction(menu_file, "Add Edge", NULL, AddEdge, NULL);

		AG_MenuSeparator(menu_file);

		AG_MenuAction(menu_file, "Quit", NULL, QuitApplication, NULL);

#ifdef ENABLE_DEBUGGER
		AG_MenuSeparator(menu_file);
		AG_MenuAction(menu_file, "Launch Debugger", NULL, LaunchDebugger, NULL);
#endif
	}

	/* Setup the status bar at the bottom of the window */
	AG_SeparatorNew(win, AG_SEPARATOR_HORIZ);
	statusbar = AG_StatusbarNew(win, AG_STATUSBAR_HFILL);
	statuslabel = AG_StatusbarAddLabel(statusbar, "");
	AG_SetPointer(dri, "status_label_p", statuslabel);
	AG_LabelText(statuslabel, "ready");

	AG_WindowShow(win);

	AG_EventLoop();
	return (0);
}
