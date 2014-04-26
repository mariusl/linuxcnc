#!/usr/bin/env python
# -*- coding:UTF-8 -*-
"""
    This file will control some options of the custom panel
    and should be something like a skeleton to begin with
    Please include a smal description of the file, so it is much
    easier for other developers to follow yozr style. there ar many 
    lasy people out there, not taking care of that, please do not become 
    one of them!

    Copyright 2013 Norbert Schechner
    nieson@web.de

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

"""

# Please read also the above text! And if you begin to code some new stuff
# please include comments, so others can follow you!

# Name varialbles so others do now there meaning, see the following example:
# v=f-s+(w.get_value)
# value = first_value - second_value + (widget.get_value)
# it cost no performanca, but is much easier to read!
# for coding style I recfommend to follow PEP8
# see : http://legacy.python.org/dev/peps/pep-0008/


####################################################################
# Watch the terminal output, as the error reports will apear there #
####################################################################

# We will need to import some basic stuff
import hal           # needed to make our own hal pins
import hal_glib      # needed to make our own hal pins and react with handlers

class HidingPanel:

    # __init__ will set the defaults and get all relevant information, before showing the panel
    def __init__(self, halcomp, builder, useropts):
        self.builder = builder
        self.halcomp = halcomp

        # lets make our hal pins, to find out what pins you can build, please take a look at
        # http://www.linuxcnc.org/docs/devel/html/hal/halmodule.html#_create_pins_and_parameters
        # This way we are able to handle events here
        pin = hal_glib.GPin(self.halcomp.newpin("hide-panel", hal.HAL_BIT, hal.HAL_IN))
        pin.connect("value_changed", self._hide_panel_changed)

        # and this way we just can read or write the pin values, but not react to the changes
        self.halcomp.newpin("panel-is-hidden", hal.HAL_BIT, hal.HAL_OUT)


        # if we want to have handlers to our widgets, or we want to modify them, we
        # need to get there instance with the builder
        # we get the needed widgets and connect them like so
        self.vbox_main = self.builder.get_object("vbox_main")
        self.lbl_text = self.builder.get_object("lbl_text")
        self.lbl_pin_value = self.builder.get_object("lbl_pin_value")

        # Oh there is a button we will connect the pressed event to our own handler
        self.btn_set_text_to_label = self.builder.get_object("btn_set_text_to_label")
        self.btn_set_text_to_label.connect("pressed", self._btn_set_text_to_label_pressed)

        # and this one to the clicked event
        self.btn_hide = self.builder.get_object("btn_hide")
        self.btn_hide.connect("clicked", self._btn_hide_clicked)

        self.btn_show_pin_value = self.builder.get_object("btn_show_pin_value")
        self.btn_show_pin_value.connect("clicked", self._on_btn_show_pin_value_clicked)

# OK here comes our handlers to react to the hal pin changes
    def _hide_panel_changed(self, pin):
        # what to do if the pin is height?
        if pin.get():
            # OK we want to hide the panel, but we could not hide its parrent, because
            # an embedded panel is reparented, so we hide the vbox_main instead
            self.vbox_main.hide()
            self.halcomp["panel-is-hidden"] = True

        # if the pin is low, we want to show the panel
        else:
            self.vbox_main.show()
            self.halcomp["panel-is-hidden"] = False


# OK here comes our handlers to react to the button
    def _btn_set_text_to_label_pressed(self, widget):
        # widget = the widget, what does send the signal in the case btn_set_text_to_label
        text = self.lbl_text.get_text()
        text += " with added text"
        self.lbl_text.set_text(text)

    def _btn_hide_clicked(self, widget):
        self.vbox_main.hide()

    def _on_btn_show_pin_value_clicked(self, widget):
        text = self.halcomp["panel-is-hidden"]
        self.lbl_pin_value.set_text(str(text))


# This is the main procedure in this case, please take care that the name within the return value
# is exactly the same as the Class Name, (case senstitive)
def get_handlers(halcomp, builder, useropts):
    return(HidingPanel(halcomp, builder, useropts))
