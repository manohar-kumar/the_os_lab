######################################################
CS 377 - Lab Assignment 3 (A)

23 January 2015
Prof D. M. Dhamdhere

######################################################
	
To compile run  'make'
To execute run executable 'jash'

Run 'make clean' to remove executables and .o files

######################################################
	
Create a single tar-zipped archive of your folder (which you have to submit) —
tar ­zcvf <RollNumber1>_<RollNumber2>_lab3a.tgz <RollNumber1>_<RollNumber2>_lab3a


######################################################	
Team : 	120050044 Manohar kumar
	120050076 Shashank.B

Implememtation status : 
	cd cmd works
	run <filename> works
	parallel execution works
	sequential execution works
	exit (exits jash; signals ctrl+c,ctrl+\ will kill 
	     any child foreground process if any but not jash)
	file execution works
	sequential_or works

	compound statements also work(
	-> “run” or “sequential” commands within a batch file provided to “run”
	-> sequential cd test ::: pwd ::: ls
	-> parallel ls ::: run test/test2.run ::: pwd
	)
testcases:

	compund statements can be tested in test/test3.run
	others in test1.run,test2.run which were given

