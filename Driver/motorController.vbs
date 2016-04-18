Set wshShell = CreateObject( "WScript.Shell" )
Dim sCurPath
sCurPath = wshShell.CurrentDirectory
Set lvapp = CreateObject("LabVIEW.Application")

Wscript.Echo sCurPath & "\LabView\Arduino Motor shield Pietro\Diamond Stepper Motor.vi"
Set viMotor = lvapp.GetVIReference(sCurPath & "\LabView\Arduino Motor shield Pietro\Diamond Stepper Motor.vi") 'Load the VI in memory

viMotor.FPWinOpen = True 'show the front panel

Dim paramNames(1), paramVals(1)
Dim motorParamPanel

paramNames(0) = "Step Motor Param"
paramNames(1) = "action"


'fisso il valore della tensione
motorParamPanel = viMotor.GetControlValue("Step Motor Param") 'Prendo dal VI il cluster motorParamPanel
motorParamPanel(0) = CInt(wshShell.ExpandEnvironmentStrings("%motorSpeed%"))  'creo un int di valore pari a quello della variabile di ambiente motorSpeed
motorParamPanel(1) = CInt(wshShell.ExpandEnvironmentStrings("%numberOfStep%")) ' numero di step
paramVals(0) = motorParamPanel 'Passo il cluster nelle nelleParamVals

paramVals(1) = CInt(3)

' lancio labview passandogli i parametri con i valori fissati
viMotor.Call paramNames,paramVals 'run the VI

Wscript.Echo "Script terminato"

