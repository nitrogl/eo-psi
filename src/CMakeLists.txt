project(hashbuckets)


add_executable(zzrndgen zzrndgen.cpp)
target_link_libraries (zzrndgen ntl)

add_executable(hbtest strint simplebm outofboundex murmurhash3 shazzp hbtest.cpp)
target_link_libraries (hbtest ntl gmpxx gmp cryptopp pthread) # only POSIX has got pthreads, win has not

install(TARGETS hbtest RUNTIME DESTINATION bin)
install(TARGETS zzrndgen RUNTIME DESTINATION bin)
