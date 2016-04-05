Set wshShell = CreateObject( "WScript.Shell" )
Dim sCurPath
sCurPath = wshShell.CurrentDirectory
Set lvapp = CreateObject("LabVIEW.Application")
Set vi = lvapp.GetVIReference(sCurPath & "\LabView\TEST_DT5521 - waitCase\Test_DT5521.vi") 'Load the VI in memory

Wscript.Echo "dentro il vbscript"

vi.FPWinOpen = True 'show the front panel

Dim paramNames(6), paramVals(6)
Dim channelPanel, chArray, voltageValue  
Dim channelPanel2, chArray2
Dim switchPanel, chOnValue
Dim switchPanel2, chOffValue
Dim chButton

paramNames(0) = "CHANNEL SETTING"
paramNames(1) = "SWITCH"
paramNames(2) = "channelSelector"
paramNames(3) = "CHANNEL SETTING 2"
paramNames(4) = "SWITCH 2"
paramNames(5) = "channelSelector 2"
paramNames(6) = "timePowerSupplyON"



'fisso il valore della tensione
channelPanel = vi.GetControlValue("CHANNEL SETTING") 'Prendo dal VI il cluster CHANNEL
voltageValue = CDbl(wshShell.ExpandEnvironmentStrings("%voltageValue%"))  'creo un double di valore pari a quello della variabile di ambiente voltage
chArray= channelPanel(0) 'Prendo il Channel 1 dal Cluster
chArray(0) = voltageValue 'Assegno il valore 1 alla prima variabile (Voltage) del channel 1
channelPanel(0)=chArray 'Riassengno tutto il vettore chArray al cluster nella posizione 1
paramVals(0) = channelPanel 'Passo il cluster nelle nelleParamVals

'fisso il valore della tensione - 2
channelPanel2 = vi.GetControlValue("CHANNEL SETTING2") 'Prendo dal VI il cluster CHANNEL
chArray2= channelPanel2(0) 'Prendo il Channel 1 dal Cluster
chArray2(0) = voltageValue 'Assegno il valore 1 alla prima variabile (Voltage) del channel 1
channelPanel2(0)=chArray2 'Riassengno tutto il vettore chArray al cluster nella posizione 1
paramVals(3) = channelPanel2 'Passo il cluster nelle nelleParamVals


' abilito il canale 
switchPanel = vi.GetControlValue("SWITCH") 'Prendo dal VI il cluster SWITCH
chOnValue = CInt(wshShell.ExpandEnvironmentStrings("%chOnValue%"))
'chOnValue = CInt(1)
switchPanel(0) = chOnValue
paramVals(1) = switchPanel 'Passo il cluster nelle nelleParamVals

' spengo il canale
switchPanel2 = vi.GetControlValue("SWITCH 2") 'Prendo dal VI il cluster SWITCH 2
chOffValue = CInt(0)
switchPanel2(0) = chOffValue
paramVals(4) = switchPanel2 'Passo il cluster nelle nelleParamVals


' equivalente del click sul bottone del front panel
paramVals(2) = CDbl(wshShell.ExpandEnvironmentStrings("%chButton%"))

' equivalente del click sul bottone del front panel - channelSelector2
paramVals(5) = CDbl(wshShell.ExpandEnvironmentStrings("%chButton%"))


' imposto tempo in cui devo alimentare il diamante
paramVals(6) = CDbl(wshShell.ExpandEnvironmentStrings("%timePowerSupplyOn%"))


' lancio labview passandogli i parametri con i valori fissati
vi.Call paramNames,paramVals 'run the VI

Wscript.Echo "Script terminato"

