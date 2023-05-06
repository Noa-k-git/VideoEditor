from typing import Tuple, List

class FormatError(Exception):
    """Exception raised for errors in the message format"""
    pass

class Protocol:
    # Protocol Constants
    NUM_FIELD_LENGTH = 8 # Exact length of num field (in bytes)
    CMD_FIELD_LENGTH = 16	# Exact length of cmd field (in bytes)
    # STATUS_FIELD_LENGTH = 2
    SIZE_FIELD_LENGTH = 8   # Exact length of length field (in bytes)
    MAX_DATA_LENGTH = 10**SIZE_FIELD_LENGTH-1  # Max size of data field according to protocol
    MSG_HEADER_LENGTH = CMD_FIELD_LENGTH + 1 + SIZE_FIELD_LENGTH + 1# + STATUS_FIELD_LENGTH + 1  # Exact size of header (CMD+LENGTH fields)
    MAX_MSG_LENGTH = MSG_HEADER_LENGTH + MAX_DATA_LENGTH  # Max size of total message
    PART_SIZE = 2048
    DELIMITER = "|"  # Delimiter character in protocol
    
    # Separators between args: 
    #   1) ;    (x,x,x) --> x;x;x
    #   2) *    ((x,x),(x,x),(x,x)) --> x*x;x*x;x*x
    #   3) >    (y, (x,x, (z,z,z)), y) --> y;x*x>z>z>z;y
    SEPARATORS = (';', '*', '>')

    CMD_STATUS = {True : 'OK', False : 'FA'}
    COMMANDS = ['SIGNUP', 
                'LOGIN', 
                'LOGOUT', 
                'PULLINFO',
                'CREATE',
                'ADDUSERS',
                'REMOVEUSER',
                'PUSHPROJECT',
                'PULLPROJECT'                    
    ]
    
    def parse_data(data:bytes) -> Tuple[bool, str, bytes]:
        """Receives data, parses the data to command & message

        Args:
            data (bytes): the data from the user

        Returns:
            Tuple[bool, str, bytes]: (succeed, cmd, message)
            bool -> whether the function run without errors, if errors occurred,
                    returns False and the message returned will be the error message
            str ->  the command in the header
            bytes -> the message
            
        """
        cmd = ""
        succeed = True
        b_header, b_message = data[:Protocol.MSG_HEADER_LENGTH], data[Protocol.MSG_HEADER_LENGTH:]
        try:
            header = b_header.decode()
            if header.count(Protocol.DELIMITER) != 2:
                raise(FormatError)
            header_fields = header.split(Protocol.DELIMITER)
            cmd = header_fields[0]
            length = int(header_fields[1])
            if length != len(b_message):
                succeed = False
                b_message = b'Message corrupted'
        except:
            succeed = False
            b_message = b'Header format'

        
        return succeed, cmd.strip(), b_message

    #def parse_header(header:bytes) -> 
    def parse_message(message:str) -> List[str]:
        """Receives the message part of a request from the user and return a list of arguments passed by the user

        Args:
            message (str): the message part of a request from the user

        Returns:
            List(str): the args in the request in a form of list.
        """
        for separator in Protocol.SEPARATORS:
            if separator in message:
                parts = message.split(separator)
                if len(parts) == 1:
                    return message
                else:
                    return [Protocol.parse_message(part) for part in parts]
        return message
        
    def build_message(message_lst:list)->str:
        """Receives a list of arguments that should be sent and convert them to a string to send in the message part according to the protocol

        Args:
            message_lst (list): list of arguments
        
        Returns:
            str: the string of message to be sent
        """
        def separator_level(s, idx):
            return s[:idx].count('[') - s[:idx].count(']') - 1
        message = str(message_lst)
        # if isinstance(message_lst, list):
        #     message = message[1:-1]
        message = message.replace('(', '[').replace(')', ']')
        message = message.replace("'", '').replace(', ', ',')
        
        for idx in range(len(message)):
            if message[idx] == ',':
                message = message[:idx] + Protocol.SEPARATORS[separator_level(message, idx)] + message[idx + 1:]
        return message.replace('[', '').replace(']', '')

    def pad_field(field, wanted_length:int, char:str = '0'):
        """pad a field to the wanted length with the char specified.

        Args:
            field (str): the field content
            wanted_length (int): teh field length
            char (str, optional): pad a string with the char, if 0 will pad from the left else from the right. Defaults to '0'.
        """
        buffer = char * (wanted_length - len(str(field)))
        if char == '0':
            return  buffer + str(field)
        return str(field) + buffer
        
    def build_response(cmd:str, code:bool, message:str) -> List[str]:
        """Receives command name, the status code of the cmd and a message and build a list for response

        Args:
            cmd (str): command
            code (bool): whether the command was successful or not
            message (str): the message to the user

        Returns:
            List[str]: the response from the server according to protocol (command, status_code, length, message)
                       if the message is bigger then the maximum size, a list of all the parts to be sent
            None if error occurred
        """
        response_lst = []
        response_lst.append(Protocol.pad_field(cmd, Protocol.CMD_FIELD_LENGTH, ' '))
        # response_lst.append(cmd)
        # response_lst[0] = response_lst[0] + ' ' * (Protocol.CMD_FIELD_LENGTH - len(response_lst[0]))
        if code in Protocol.CMD_STATUS:
            response_lst.append(Protocol.CMD_STATUS[code])
        else:
            response_lst.append(code)
        
        length = len(str(message))
        if length > Protocol.MAX_DATA_LENGTH:
            message = "Message is too big"
            length = len(message)
        
        response_lst.append(Protocol.pad_field(length, Protocol.SIZE_FIELD_LENGTH))
        response_lst.append(message)
        # response_lst.append('0' * (Protocol.SIZE_FIELD_LENGTH - len(str(length))) + str(length))

        full_msg = Protocol.join_response_fields(response_lst)
        parts = []
        counter = 1
        while full_msg:
            slicer = min(len(full_msg), Protocol.PART_SIZE-Protocol.NUM_FIELD_LENGTH)
            parts.append((Protocol.pad_field(counter, Protocol.NUM_FIELD_LENGTH, '0') + full_msg[:slicer]).encode())
            full_msg = full_msg[slicer:]
            counter += 1
        else:
            parts.append((Protocol.pad_field(counter, Protocol.NUM_FIELD_LENGTH, '0')).encode())
        #     if len(full_msg) > Protocol.PART_SIZE:
        #         counter+=1
        #         parts.append((Protocol.pad_field(counter, Protocol.NUM_FIELD_LENGTH, '0') + full_msg[:Protocol.PART_SIZE-Protocol.NUM_FIELD_LENGTH]).encode())
        #         full_msg = full_msg[Protocol.PART_SIZE:]
        #     else:
        #         parts.append(full_msg.encode())
        #         full_msg = ''
        return parts
    
    def join_response_fields(msg_fields:list)->str:
        """
        Helper method. Gets a list, joins all of it's fields to one string divided by the delimiter. 
        Args:
            msg_fields (list): list to join.
        Returns: 
            str: string that looks like cell1|cell2|cell3
        """
        msg_fields = (str(field) for field in msg_fields)
        return Protocol.DELIMITER.join(msg_fields)
