## Crash Course in GIT 

**git clone git@gitlab.ethz.ch:bhadaura/OptimisationProject.git** to download 
the code from github. Needed just once. Or just download using the link.

**git add filename.cpp** to add any file to the staging area. This tells git to keep
a track of the file and the subsequent changes made to it. 

**git commit -m "mandatory commit message"** to commit changes you've made to the file. 
This only affects your local copy of the code

**git push origin master** to push your changes to the remote repository (online). 

**git pull origin master** to update local copy with latest version of code. 
Sometimes it might ask you to perform a *git pull* first if there are newer 
versions of the code online, than your local copy. Good idea to perform a pull 
everytime *before* you start working on code (not a good idea professionally, 
but it doesnt matter here). 
Might need to setup a [SSH key](https://gitlab.ethz.ch/profile/keys). 
Else you might need to keep entering password everytime you pull/push.   

[15 minute interactive tutorial to get started.](https://try.github.io/levels/1/challenges/1)
Since I have skipped a lot of the getting started stuff. I used this to get started as well.

[Text Tutorial](https://git-scm.com/docs/gittutorial)

## Compilation and Execution 

* Place completed .cpp files in *src* folder. Any file not in the src folder *will not* be compiled.

* Place necessary header (.h) files in the *inc* folder

type **Make all** from terminal to compile. 

type **Make clean** to clear compiled intermediate and executable files. 

type **./optimize.out** in terminal to run the executable/

 