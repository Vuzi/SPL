# Fonction testant le PGCD

Func
[
	name = PGCD;
	type = int;

	Args
	[
		Var[ name = a; type = int; ]
		Var[ name = b; type = int; ]
	]

	Code
	[
		if( a%b == 0)
		[	
			return(b);
		]
		else
		[
			return(PGCD(b, a%b));
		]
	]
]



# 'Main' du programme
start
[
	Var[ name = a; type = int; ]
	Var[ name = b; type = int; ]

	in(a, b);

	out(PGCD(a,b));
]
