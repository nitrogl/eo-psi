project(hashbuckets)


add_executable(zzrndgen zzrndgen.cpp)
target_link_libraries (zzrndgen ntl)

add_executable(hbtest strint bucket simplebm outofboundex hashbuckets murmurhash3 test.cpp)
target_link_libraries (hbtest ntl gmpxx gmp)

install(TARGETS hbtest RUNTIME DESTINATION bin)
install(TARGETS zzrndgen RUNTIME DESTINATION bin)
