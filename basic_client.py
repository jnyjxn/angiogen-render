import requests
from PIL import Image
import binascii

r = requests.get('http://172.17.0.2:5000/xray')

bs = binascii.a2b_base64(r.json()["images"][0][0])

print(len(bs))

i = Image.frombytes('RGB', (1496, 1526), bs, 'raw')
i.show()
# i = Image.open(BytesIO(r.content))
# i.show()