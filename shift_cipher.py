def encrypt(text,s):
    result = ""
 
    # traverse text
    for i in range(len(text)):
        char = text[i]
 
        # Encrypt uppercase characters
        result += chr((ord(char) + s))
 
    return result
    
def decrypt(text, s):
    return encrypt(text, -s)