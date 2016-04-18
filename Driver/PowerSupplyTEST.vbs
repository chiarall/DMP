Set wshShell = CreateObject( "WScript.Shell" )
Dim sCurPath
sCurPath = wshShell.CurrentDirectory
Set lvapp = CreateObject("LabVIEW.Application")
'Set vi = lvapp.GetVIReference(sCurPath & "\LabView\PowerSupplyPietro\TEST_DT5521\Test_DT5521.vi") 'Load the VI in memory
Set vi = lvapp.GetVIReference(sCurPath & "\LabView\TEST_DT5521_ModPietro_v2\Test_DT5521.vi") 'Load the VI in memory

Wscript.Echo "dentro il vbscript"

vi.FPWinOpen = True 'show the front panel

Dim paramNames(1), paramVals(1)
Dim paramPathFiles, pathCHzero, pathCHone, pathCHtwo, pathCHthree, pathCNTRL

paramNames(0) = "Parameter path"
paramNames(1) = "LOAD PARM. FILE"


'fisso il valore della tensione
paramPathFiles = vi.GetControlValue("Parameter path") 'Prendo dal VI il cluster Parameter path
pathCHzero = CStr(wshShell.ExpandEnvironmentStrings("%pathCHzero%"))  'creo un double di valore pari a quello della variabile di ambiente voltage
paramPathFiles(0) = pathCHzero 'Prendo il Channel 1 dal Cluster
pathCHone = CStr(wshShell.ExpandEnvironmentStrings("%pathCHone%"))  'creo un double di valore pari a quello della variabile di ambiente voltage
paramPathFiles(1) = pathCHone 'Prendo il Channel 1 dal Cluster
pathCHtwo = CStr(wshShell.ExpandEnvironmentStrings("%pathCHtwo%"))  'creo un double di valore pari a quello della variabile di ambiente voltage
paramPathFiles(2) = pathCHtwo 'Prendo il Channel 1 dal Cluster
pathCHthree = CStr(wshShell.ExpandEnvironmentStrings("%pathCHthree%"))  'creo un double di valore pari a quello della variabile di ambiente voltage
paramPathFiles(3) = pathCHthree 'Prendo il Channel 1 dal Cluster
pathCNTRL = CStr(wshShell.ExpandEnvironmentStrings("%pathCNTRL%"))  'creo un double di valore pari a quello della variabile di ambiente voltage
paramPathFiles(4) = pathCNTRL 'Prendo il Channel 1 dal Cluster
paramVals(0) = paramPathFiles 'Passo il cluster nelle nelleParamVals


paramVals(1) = CBool(1)

' lancio labview passandogli i parametri con i valori fissati
vi.Call paramNames,paramVals 'run the VI

Wscript.Echo "Script terminato"

