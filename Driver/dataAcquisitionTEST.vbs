Set wshShell = CreateObject( "WScript.Shell" )
Dim sCurPath
sCurPath = wshShell.CurrentDirectory
Set lvapp = CreateObject("LabVIEW.Application")
Set viAcquisition = lvapp.GetVIReference(sCurPath & "\LabView\AH501B TCP-IP Control\AH501B TCP-IP Control.vi") 'Load the VI in memory

Wscript.Echo "dentro il vbscript"

viAcquisition.FPWinOpen = True 'show the front panel

Dim paramNames(6), paramVals(6) 
paramNames(0) = "timeout"
paramNames(1) = "Numeric"
paramNames(2) = "Save data Cluster"
paramNames(3) = "Path"
paramNames(4) = "timeToStop" 'timer di stop
paramNames(5) = "enableTimer" 'boolean to enable timer
paramNames(6) = "Acquisition time (ms):"

' skip attesa pressione bottone start (sempre a 0 da programma)
paramVals(0) = CInt(0)

' number of channel
paramVals(1) = CInt(0)

' click su save and append
saveControl = viAcquisition.GetControlValue("Save data Cluster") 'Prendo dal VI il Bottone
Wscript.Echo UBound(saveControl)
saveControl(1) = CBool(1)
paramVals(2) = saveControl

' path del file di output
paramVals(3) = CStr(wshShell.ExpandEnvironmentStrings("%filePath%"))

' numero di cicli
paramVals(4) = CInt (wshShell.ExpandEnvironmentStrings("%numberOfCycles%"))

' abilita timer di uscita (sempre a 1 da programma)
paramVals(5) = CBool(1)

' fissa il tempo di una singola acquisizione che corrisponde al singolo valore nel file di output
paramVals(6) = CInt (wshShell.ExpandEnvironmentStrings("%singleAcquisitionTime%"))


' lancio labview passandogli i parametri con i valori fissati
viAcquisition.Call paramNames,paramVals 'run the VI

Wscript.Echo "Script terminato"

