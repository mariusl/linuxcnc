

DISPLAY_DIR=display
STEPPER_DIR=stepper
MESA_DIR=mesa
COM_DIR=common
CONTROL_DIR=control
OUT_FILE=output

plasma_stepper= $(COM_DIR)/pre_amble.sec \
								$(DISPLAY_DIR)/display_gmoccapy_plasma.sec \
								$(CONTROL_DIR)/rs274ngc.sec \
								$(CONTROL_DIR)/motion_3axis.sec \
								$(STEPPER_DIR)/stepper_3.sec \
								$(CONTROL_DIR)/emcio_plasma.sec

mill_stepper_3axis= $(COM_DIR)/pre_amble.sec \
								$(DISPLAY_DIR)/display_gmoccapy_3axis.sec \
								$(CONTROL_DIR)/rs274ngc_mill.sec \
								$(CONTROL_DIR)/motion_3axis.sec \
								$(STEPPER_DIR)/stepper_3.sec \
								$(CONTROL_DIR)/emcio.sec

								
plasma_stepper:$(plasma_stepper)
								@cat $(plasma_stepper) > plasma_stepper.ini			
								@#cat $(plasma_stepper) > $(OUT_FILE).ini			
								@echo "Created plasma_stepper.ini\r\n"
								
mill_stepper_3axis:$(mill_stepper_3axis)
								@cat $(mill_stepper_3axis) > mill_stepper_3axis.ini
								@#cat $(mill_stepper_3axis) > $(OUT_FILE).ini
								@echo "Created mill_stepper_3axis.ini\r\n"																
																								
																								
?: 
	@echo "These are the options that you have: Call make with any of them as parameter"
	@echo "plasma_stepper"
	@echo "mill_stepper_3axis"
																								
