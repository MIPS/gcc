		if (y .ne. 1.) call abort
		if (abs(y - .1197949) .ge. 1.e-6) call abort
		if (y .gt. 1.5e-44) call abort
		if (y1 .ne. 1.) call abort
		if (abs(y1 - .1197949d0) .ge. 1.d-6) call abort
		if (y1 .gt. 1.5d-44) call abort
