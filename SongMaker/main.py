def create_cstyle_list(arg):
    out = "{"
    for data in arg:
        out += str(data) + ","
    out += "}"
    return out


def string_division_to_float(arg: str):
    numbers = arg.split("/")
    return float(numbers[0]) / float(numbers[1])


fileToParse = open("FileToParse.txt", "r")

songIndex: str = input("input your song index: ")

notes = []
lengths = []
directions = []

for line in fileToParse.readlines():
    mode = 0  # 0=notes,1=lengths,2=directions
    line = line.replace(" ", "")
    line = line.strip("\n")
    for linePart in line.split(","):
        if mode == 0:
            notes.append(linePart)
        if mode == 1:
            lengths.append(string_division_to_float(linePart))
        if mode == 2:
            directions.append(linePart)
        mode += 1

print("int songNotes" + songIndex + "[] =", create_cstyle_list(notes) + ";")
print("float songTimings" + songIndex + "[] =", create_cstyle_list(lengths) + ";")
print("int songDirections" + songIndex + "[] =", create_cstyle_list(directions) + ";")
input("finished")
