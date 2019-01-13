#ifndef NONNEGATIVE_H
#define NONNEGATIVE_H
/** A small function to enable checking that arbitrary numbers are nonnegative
	@author Cameron Shepherd
*/
template<class T0>
bool nonNegative(T0 t0) { return (t0 >= 0); }

template<class T0, class T1, class... Args>
bool nonNegative(T0 t0, T1 t1, Args... args) {
	return nonNegative(t1, args...);
}

#endif
