default: 
	@gcc main/main.c main/functions.c spec1/spec1.c spec2/spec2.c spec3/spec3.c spec4/spec4.c spec5/spec5.c spec6/spec6.c spec7/spec7.c spec8/spec8.c spec9/spec9.c spec10_11_12/spec10_11_12.c spec13/spec13.c spec14/spec14.c spec16/spec16.c spec17/spec17.c utils/utils.c -o mysh
	@./mysh

debug:
	@gcc main/main.c main/functions.c spec1/spec1.c spec2/spec2.c spec3/spec3.c spec4/spec4.c spec5/spec5.c spec6/spec6.c spec7/spec7.c spec8/spec8.c spec9/spec9.c spec10_11_12/spec10_11_12.c spec13/spec13.c spec14/spec14.c spec16/spec16.c spec17/spec17.c utils/utils.c -g
	@lldb ./a.out

git:
	@git add .
	@git commit -m "default"
	@git push origin final
