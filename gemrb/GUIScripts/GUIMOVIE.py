import GemRB

MovieWindow = 0
TextAreaControl = 0
MoviesTable = 0

def OnLoad():
	global MovieWindow, TextAreaControl, MoviesTable
#for testing purposes
	GemRB.SetVar("BISLOGO",1)
	GemRB.SetVar("ENDCRDIT",1)
	GemRB.SetVar("INTRO15F",1)
	GemRB.SetVar("WOTC",1)
	GemRB.SetVar("DEATHAND",1)
	GemRB.SetVar("BWDRAGON",1)
	GemRB.SetVar("RESTCAMP",1)
	GemRB.SetVar("DAYNITE",1)
	GemRB.SetVar("NITEDAY",1)
	GemRB.SetVar("RESTDUNG",1)
	GemRB.SetVar("RESTINN",1)
	GemRB.SetVar("POCKETZZ",1)

	GemRB.LoadWindowPack("GUIMOVIE")
	MovieWindow = GemRB.LoadWindow(0)
	TextAreaControl = GemRB.GetControl(MovieWindow, 0)
	PlayButton = GemRB.GetControl(MovieWindow, 2)
	CreditsButton = GemRB.GetControl(MovieWindow, 3)
	DoneButton = GemRB.GetControl(MovieWindow, 4)
	MoviesTable = GemRB.LoadTable("MOVIDESC")
	for i in range(0, GemRB.GetTableRowCount(MoviesTable) ):
		t = GemRB.GetTableRowName(MoviesTable, i)
		if GemRB.GetVar(t)==1:
			s = GemRB.GetTableValue(MoviesTable, i, 0)
			GemRB.TextAreaAppend(MovieWindow, TextAreaControl, s,-1)
	GemRB.SetVarAssoc(MovieWindow, TextAreaControl, "MovieIndex",0)
	GemRB.SetText(MovieWindow, PlayButton, 17318)
	GemRB.SetText(MovieWindow, CreditsButton, 15591)
	GemRB.SetText(MovieWindow, DoneButton, 11973)
	GemRB.SetEvent(MovieWindow, PlayButton, IE_GUI_BUTTON_ON_PRESS, "PlayPress")
	GemRB.SetEvent(MovieWindow, CreditsButton, IE_GUI_BUTTON_ON_PRESS, "CreditsPress")
	GemRB.SetEvent(MovieWindow, DoneButton, IE_GUI_BUTTON_ON_PRESS, "DonePress")
	GemRB.SetVisible(MovieWindow,1)
	return
	
def PlayPress():
	print "MovieIndex", GemRB.GetVar("MovieIndex")
	s = GemRB.GetVar("MovieIndex")
	for i in range(0, GemRB.GetTableRowCount(MoviesTable) ):
		t = GemRB.GetTableRowName(MoviesTable, i)
		if GemRB.GetVar(t)==1:
			if s==0:
				s = GemRB.GetTableRowName(MoviesTable, i)
				GemRB.PlayMovie(s)
				GemRB.InvalidateWindow(MovieWindow)
				return
			s = s - 1
	return

def CreditsPress():
	return

def DonePress():
	GemRB.UnloadWindow(MovieWindow)
	GemRB.SetNextScript("Start")
	return
