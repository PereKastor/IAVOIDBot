import json
import sys
import urllib.request

if len(sys.argv) != 4:
    sys.exit(0)


data = []
dataSize = len(data)
with open(sys.argv[1]) as f:
    for line in f:
        data.append(line)

gameData = "var data = '"
gameData += data[0][:-2]
gameData += ",\"TurnsInfo\":["

index = 1
for turnData in data[1:]:
    gameData += turnData[:-1]
    index += 1
    if index < len(data):
            gameData += ","


gameData += "]}'"

target = open(sys.argv[2], 'w')
target.write(gameData)
target.close()

urllib.request.urlretrieve("http://aiguru.net/AIBootCampViewer/AIBootCampViewer.html", sys.argv[3])