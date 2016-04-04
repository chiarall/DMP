Set wshShell = CreateObject( "WScript.Shell" )
Dim sCurPath
sCurPath = wshShell.CurrentDirectory
Set lvapp = CreateObject("LabVIEW.Application")

Wscript.Echo sCurPath & "\LabView\ArduinoMotorShield\Diamond Stepper Motor.vi"
Set viMotor = lvapp.GetVIReference(sCurPath & "\LabView\ArduinoMotorShield\Diamond Stepper Motor.vi") 'Load the VI in memory

viMotor.FPWinOpen = True 'show the front panel

Dim paramNames(2), paramVals(2)
paramNames(0) = "Set Speed (Steps per Second)"
paramNames(1) = "# of Steps to Move"
paramNames(2) = "action"
paramVals(0) = CInt(wshShell.ExpandEnvironmentStrings("%motorSpeed%"))  ' velocità motore
paramVals(1) = CInt(wshShell.ExpandEnvironmentStrings("%numberOfStep%")) ' numero di step
paramVals(2) = CInt(3)

' lancio labview passandogli i parametri con i valori fissati
viMotor.Call paramNames,paramVals 'run the VI

Wscript.Echo "Script terminato"

