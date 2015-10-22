from Crypto.PublicKey import RSA

key64 = open("../config/rsa_1024_priv.pem", 'rb').read()
rsa_object = RSA.importKey(key64)

def check_sub_key(sub_key, key):
	return reduce(lambda x,y: x*y, sub_key, 1) == int(key)

def test_conception(sub_key, sign):
	sub_key_sign = [rsa_object.sign(current_sub_key, None)[0] for current_sub_key in sub_key]
	return reduce(lambda x,y: x*y, sub_key_sign, 1) % rsa_object.n == sign

key = 54659566549627989538829362159
sign = rsa_object.sign(key, None)[0]
print "SIGN =", sign

sub_key = [558139, 525163, 950161, 589181, 333107]
print "CHECK SUB_KEY", check_sub_key(sub_key, key)
print "TEST CONCEPTION", test_conception(sub_key, sign)


'''
p = 0xc7a2a062ee60af7e4e4e666c9086bd74bba08777c352f943034df20bfb3d6ef1
q = 0xc1ad9b926036d4461124774460ad21662d8ff68a46deb5c12dc993a03c07e959
n = 0x9708fd22957518618db424d5ddb66d1127b7dffe38dddbbf631a31b9088d88383503ca23b06d600b56a46a1b0bd820d3e6ad76d1df57435949ef0c23c6ebeac9
e = 0x10001
d = 0x4a6157e077404270908b9df4610add2daca4c63ff99b710410f72e73947db70e4d82d0cb90ccf47d527169583ba49704db3977834ad205f9de910d9cc5f6df81
fi = (p-1)*(q-1)

def check_sign(sign, key):
	return pow(sign, e, n) == key

def check_sub_key(sub_key, key):
	return reduce(lambda x,y: x*y, sub_key, 1) == key

def test_conception(sub_key, sign):
	sub_key_sign = [pow(current_sub_key, d, n) for current_sub_key in sub_key]
	return reduce(lambda x,y: x*y, sub_key_sign, 1) % n == sign

key = 45453347260886758277300987957
sign = pow(key,d,n)
print "SIGN =", sign
print "CHECK SIGN", check_sign(sign, key)

sub_key = [644123, 750137, 379123, 738163, 336143]
print "CHECK SUB_KEY", check_sub_key(sub_key, key)
print "TEST CONCEPTION", test_conception(sub_key, sign)
'''