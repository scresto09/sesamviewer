#ifndef MENU_H_
#define MENU_H_ 1

const gchar G_sesam_menu[]= "<ui> "
"  <menubar>"
"    <menu name=\"FileMenu\" action=\"FileMenuAction\">"
"      <menuitem name=\"New\" action=\"New2Action\" />"
"      <placeholder name=\"FileMenuAdditions\" />"
"    </menu>"
"    <menu name=\"JustifyMenu\" action=\"JustifyMenuAction\">"
"      <menuitem name=\"Left\" action=\"justify-left\"/>"
"      <menuitem name=\"Centre\" action=\"justify-center\"/>"
"      <menuitem name=\"Right\" action=\"justify-right\"/>"
"      <menuitem name=\"Fill\" action=\"justify-fill\"/>"
"    </menu>"
"  </menubar>"
"  <toolbar action=\"toolbar1\">"
"    <placeholder name=\"JustifyToolItems\">"
"      <separator/>"
"      <toolitem name=\"Left\" action=\"justify-left\"/>"
"      <toolitem name=\"Centre\" action=\"justify-center\"/>"
"      <toolitem name=\"Right\" action=\"justify-right\"/>"
"      <toolitem name=\"Fill\" action=\"justify-fill\"/>"
"      <separator/>"
"    </placeholder>"
"  </toolbar>"
"</ui>";


#endif
