
repetitions = 1000
u_initial = 0.0001
m_sd = 0.02


u = rep(u_initial, repetitions)

for(generation in 1:1000){
	u = u * rnorm(repetitions, 1, m_sd)
}

hist(u)