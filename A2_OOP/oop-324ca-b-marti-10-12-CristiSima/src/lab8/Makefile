JARGS= -cp .;"../libs/*"

Student_build:
	cd .. && javac $(JARGS) lab8/Student.java

Student_buildTest:
	cd .. && javac $(JARGS) lab8/StudentTest.java

Student_run: Student_build
	cd .. && java $(JARGS) lab8.Student

# Student_test: Student_buildTest
#	 cd .. && cd && java $(JARGS) -jar ../libs/junit-platform-commons-1.5.2.jar lab8.StudentTest

Student: Student_run #Student_test

.PHONY: Student Student_run #Student_test
