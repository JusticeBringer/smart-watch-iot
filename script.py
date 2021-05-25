import requests
import random
import time
while True:
    time.sleep(30)
    x = random.randrange(0, 101, 2)
    response = requests.post('http://localhost:9080/settings/brightness/' + str(x))
