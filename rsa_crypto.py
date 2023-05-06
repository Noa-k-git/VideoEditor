from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
import base64

def generate_keys():
    # RSA modulus length must be a multiple of 256 and >= 1024
    modulus_length = 256*4  # use larger value in production
    privatekey = RSA.generate(modulus_length, e=65537)
    publickey = privatekey.publickey()
    return privatekey, publickey

def encrypt_message(a_message, publickey):
    cipher = PKCS1_OAEP.new(publickey)
    encrypted_msg = cipher.encrypt(a_message.encode())
    encoded_encrypted_msg = base64.b64encode(encrypted_msg).decode()  # base64 encoded strings are database friendly
    return encoded_encrypted_msg

def decrypt_message(encoded_encrypted_msg, privatekey):
    decoded_encrypted_msg = base64.b64decode(encoded_encrypted_msg.encode())
    cipher = PKCS1_OAEP.new(privatekey)
    decoded_decrypted_msg = cipher.decrypt(decoded_encrypted_msg).decode()
    return decoded_decrypted_msg

########## BEGIN ##########


a_message = "a"*(26)
privatekey , publickey = generate_keys()
encrypted_msg = encrypt_message(a_message, publickey)
decrypted_msg = decrypt_message(encrypted_msg, privatekey)

# print("%s - (%d)" % (privatekey.exportKey() , len(privatekey.exportKey())))
# print("%s - (%d)" % (publickey.exportKey() , len(publickey.exportKey())))
print(" Original content: %s - (%d)" % (a_message, len(a_message)))
print("Encrypted message: %s - (%d)" % (encrypted_msg, len(encrypted_msg)))
print("Decrypted message: %s - (%d)" % (decrypted_msg, len(decrypted_msg)))