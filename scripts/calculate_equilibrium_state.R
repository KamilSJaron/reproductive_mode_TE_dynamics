getNeq <- function(a, b, u, v){
	(sqrt(2 * (a^2 + (u - v))^2) - 2 * a) / (2 * b)
}

get_b <- function(a, u, v, Neq){
	(sqrt(2 * (a^2 + (u - v))^2) - 2 * a) / (2 * Neq)
}

get_v <- function(a, b, u, Neq){
	u - sqrt((Neq^2 * 32 * b^2 / a^2) - a^2)
}

get_u <- function(a, b, v, Neq){
	sqrt((Neq^2 * 32 * b^2 / a^2) - a^2) + v
}