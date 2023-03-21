# import queue, threading
# def g(q):
#     while True:
#         print(q.get())


# a = queue.Queue()
# a.put(1)
# a.put(2)
# t = threading.Thread(target=g, args=(a,))
# t.start()

separators = (';', '*', '>')
def build_message(message_lst):
    def separator_level(s, idx):
        return s[:idx].count('[') - s[:idx].count(']') - 1
    message = str(message_lst)
    message = message.replace("'", '').replace(', ', ',')
    for idx in range(len(message)):
        if message[idx] == ',':
            message = message[:idx] + separators[separator_level(message, idx)] + message[idx + 1:]
    return message.replace('[', '').replace(']', '')


def data_info(data):
    separators = (';', '*', '>')
    for separator in separators:
        if separator in data:
            parts = data.split(separator)
            if len(parts) == 1:
                return data
            else:
                return [data_info(part) for part in parts]
    return data

message_lst = ['hello', ['world', 'foo', ['bar', 'baz']], 'qux']
result = build_message(message_lst)
print(result)
info = data_info(result)
print(message_lst)
print(info)
