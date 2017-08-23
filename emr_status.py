import boto3
import yaml
import serial
import sys
import time
from enum import Enum
from pathlib import Path


class Status(Enum):
    GREEN = b'0'
    YELLOW = b'1'
    RED = b'2'

status_map = {
    'TERMINATED': Status.GREEN,
    'WAITING': Status.GREEN,
    'TERMINATED_WITH_ERRORS': Status.RED,
    'RUNNING': Status.YELLOW
}

creds_text = Path('creds.yml').read_text()
creds_dict = yaml.load(creds_text).get('aws-creds')

aws_key = creds_dict['key']
aws_secret = creds_dict['secret']


emr = boto3.client(
    'emr',
    aws_access_key_id=aws_key,
    aws_secret_access_key=aws_secret,
    region_name='us-east-1'
)

def get_status():
    clusters = emr.list_clusters()
    status_key = clusters['Clusters'][0]['Status']['State']
    if status_key not in status_map.keys():
        return status_map['RUNNING']
    return status_map[status_key]

serial_path = sys.argv[1]
serial_connector = serial.Serial(serial_path)

while 1:
    status_code = get_status()
    print("Received status to write: {}".format(status_code))
    serial_connector.write(status_code.value)
    time.sleep(15)
