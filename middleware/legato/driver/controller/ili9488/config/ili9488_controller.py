# coding: utf-8
##############################################################################
# Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################

def instantiateComponent(comp):
	comp.setHelpFile("../../../docs/help_harmony_gfx_html_alias.h")
	#comp.setHelp("IDH_HTML_GFXLIB_ILI9488_Display_Controller_Driver_Library")

	SYS_DEFINITIONS_H = comp.createFileSymbol("SYS_DEFINITIONS_H", None)
	SYS_DEFINITIONS_H.setType("STRING")
	SYS_DEFINITIONS_H.setOutputName("core.LIST_SYSTEM_DEFINITIONS_H_INCLUDES")
	SYS_DEFINITIONS_H.setSourcePath("templates/definitions.h.ftl")
	SYS_DEFINITIONS_H.setMarkup(True)

	SYS_INIT_C = comp.createFileSymbol("SYS_INIT_C", None)
	SYS_INIT_C.setType("STRING")
	SYS_INIT_C.setOutputName("core.LIST_SYSTEM_INIT_C_SYS_INITIALIZE_DRIVERS")
	SYS_INIT_C.setSourcePath("templates/init.c.ftl")
	SYS_INIT_C.setMarkup(True)

	SYS_TASK_C = comp.createFileSymbol("SYS_TASK_C", None)
	SYS_TASK_C.setType("STRING")
	SYS_TASK_C.setOutputName("core.LIST_SYSTEM_TASKS_C_CALL_DRIVER_TASKS")
	SYS_TASK_C.setSourcePath("templates/tasks.c.ftl")
	SYS_TASK_C.setMarkup(True)

	execfile(Module.getPath() + "/config/ili9488_config.py")
	execfile(Module.getPath() + "/config/ili9488_files.py")

def onAttachmentConnected(source, target):
	if source["id"] == "Display Interface":
		print(source["component"].getID() + ": Using " + target["component"].getID() + " interface ")
		source["component"].getSymbolByID("GFX_ILI9488_DBIB_C").setEnabled(False)
		source["component"].getSymbolByID("GFX_ILI9488_SPI").setEnabled(False)
		source["component"].getSymbolByID("ParallelInterfaceWidth").setVisible(False)

		InterfaceType = str(target["component"].getSymbolByID("InterfaceType").getValue())
		source["component"].getSymbolByID("DisplayInterface").setValue(target["id"], 1)
		source["component"].getSymbolByID("DisplayInterfaceType").setValue(InterfaceType, 1)
		if InterfaceType == "SPI 4-line":
			source["component"].getSymbolByID("GFX_ILI9488_SPI").setEnabled(True)
		elif "Parallel" in InterfaceType:
			source["component"].getSymbolByID("GFX_ILI9488_DBIB_C").setEnabled(True)
			source["component"].getSymbolByID("ParallelInterfaceWidth").setVisible(True)
			if InterfaceType == "Parallel 8-bit":
				source["component"].getSymbolByID("ParallelInterfaceWidth").setValue("8-bit", True)
				source["component"].getSymbolByID("ParallelInterfaceWidth").setReadOnly(True)
			else:
				source["component"].getSymbolByID("ParallelInterfaceWidth").setReadOnly(False)
		else:
			print("Interface does not contain 'InterfaceType' capability")
