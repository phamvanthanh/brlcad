#
#			R T _ S C R I P T . T C L
#
#	Widget for producing RT script files of MGED's current view.
#
#	Author - Robert G. Parker
#

check_externs "_mged_opendb _mged_saveview"

proc init_rtScriptTool { id } {
    global mged_gui
    global rts_control

    set top .$id.do_rtScript

    if [winfo exists $top] {
	raise $top
	return
    }

    if ![info exists rts_control($id,file)] {
	regsub \.g$ [_mged_opendb] .sh default_file
	set rts_control($id,file) $default_file
    }

    if ![info exists rts_control($id,args)] {
	set rts_control($id,args) ""
    }

    toplevel $top -screen $mged_gui($id,screen)

    frame $top.gridF
    frame $top.gridF2

    label $top.fileL -text "File Name" -anchor w
    entry $top.fileE -width 12 -textvar rts_control($id,file)

    label $top.argsL -text "Other args..." -anchor w
    entry $top.argsE -width 12 -textvar rts_control($id,args)

    button $top.createB -relief raised -text "Create"\
	    -command "do_rtScript $id"
    button $top.dismissB -relief raised -text "Dismiss"\
	    -command "catch { destroy $top }"

    grid $top.fileE $top.fileL -sticky "ew" -in $top.gridF -pady 4
    grid $top.argsE $top.argsL -sticky "ew" -in $top.gridF -pady 4
    grid columnconfigure $top.gridF 0 -weight 1

    grid $top.createB x $top.dismissB -in $top.gridF2
    grid columnconfigure $top.gridF2 1 -weight 1

    pack $top.gridF $top.gridF2 -side top -expand 1 -fill both\
	    -padx 8 -pady 8

    place_near_mouse $top
    wm title $top "RT Script Tool..."
}

proc do_rtScript { id } {
    global mged_gui
    global rts_control

    cmd_win set $id
    set rts_cmd "_mged_saveview"

    if {$rts_control($id,file) != ""} {
	if [file exists $rts_control($id,file)] {
	    set result [cad_dialog .$id.rtsDialog $mged_gui($id,screen)\
		    "Overwrite $rts_control($id,file)?"\
		    "Overwrite $rts_control($id,file)?"\
		    "" 0 OK CANCEL]

	    if {$result} {
		return
	    }
	}
    } else {
	cad_dialog .$id.rtsDialog $mged_gui($id,screen)\
		"No file name specified!"\
		"No file name specified!"\
		"" 0 OK

	return
    }

    append rts_cmd " $rts_control($id,file)"

    if {$rts_control($id,args) != ""} {
	append rts_cmd " $rts_control($id,args)"
    }

    catch {eval $rts_cmd}
}