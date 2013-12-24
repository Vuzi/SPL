# Fonction calculant i puissance n

Func
[
	name = pow;
	type = int;

	Args
	[
		Var[ name = i; type = int; ]
		Var[ name = n; type = int; ]
	]

	Code
	[
		var [ name = ret; type = int; value = 1;]

		while(n > 0)
		[
			ret *= i;
			n--;
		]

		if(ret < 0)
		[
			return(-1);
		]
		else
		[
			return(ret);
		]
	]
]



# 'Main' du programme
start
[
	Var[ name = i; type = int; ]
	Var[ name = n; type = int; ]

	in(i, n);

	out(pow(i,n));
]
