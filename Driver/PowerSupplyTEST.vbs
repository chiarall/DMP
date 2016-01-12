Set wshShell = CreateObject( "WScript.Shell" )
Set lvapp = CreateObject("LabVIEW.Application")
Set vi = lvapp.GetVIReference("LabView\Test2.vi") 'Load the VI in memory

Wscript.Echo "dentro il vbscript"

vi.FPWinOpen = True 'show the front panel

Dim paramNames(2), paramVals(2)
Dim channelPanel, chArray, voltageValue  
Dim switchPanel, chOnValue
Dim chButton

paramNames(0) = "CHANNEL"
paramNames(1) = "SWITCH"
paramNames(2) = "CH0 PROG"
'paramNames(3) = "CH1 PROG"
'paramNames(4) = "CH2 PROG"
'paramNames(5) = "CH3 PROG"

'fisso il valore della tensione
channelPanel = vi.GetControlValue("CHANNEL") 'Prendo dal VI il cluster CHANNEL
voltageValue = CDbl(wshShell.ExpandEnvironmentStrings("%voltageValue%"))  'creo un double di valore pari a quello della variabile di ambiente voltage
chArray= channelPanel(0) 'Prendo il Channel 1 dal Cluster
chArray(0) = voltageValue 'Assegno il valore 1 alla prima variabile (Voltage) del channel 1
channelPanel(0)=chArray 'Riassengno tutto il vettore chArray al cluster nella posizione 1
paramVals(0) = channelPanel 'Passo il cluster nelle nelleParamVals

' abilito il canale 
switchPanel = vi.GetControlValue("SWITCH") 'Prendo dal VI il cluster SWITCH
chOnValue = CInt(wshShell.ExpandEnvironmentStrings("%chOnValue%"))
'chOnValue = CInt(1)
switchPanel(0) = chOnValue
paramVals(1) = switchPanel 'Passo il cluster nelle nelleParamVals

' click sul bottone del front panel
'chButton = CBool(1)
chButton = CBool(wshShell.ExpandEnvironmentStrings("%chButton%"))
paramVals(2) = chButton

' lancio labview passandogli i parametri con i valori fissati
vi.Call paramNames,paramVals 'run the VI

Wscript.Echo "Script terminato"

