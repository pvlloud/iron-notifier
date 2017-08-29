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


def get_creds(filename: str):
    creds_text = Path(filename).read_text()
    creds_dict = yaml.load(creds_text).get('aws-creds')
    return creds_dict['key'], creds_dict['secret']


def emr_client(aws_key: str, aws_secret: str):
    emr = boto3.client(
        'emr',
        aws_access_key_id=aws_key,
        aws_secret_access_key=aws_secret,
        region_name='us-east-1'
    )
    return emr


def get_status(emr):
    clusters = emr.list_clusters()
    status_key = clusters['Clusters'][0]['Status']['State']
    if status_key not in status_map.keys():
        return status_map['RUNNING']
    return status_map[status_key]


def main():
    aws_key, aws_secret = get_creds('creds.yml')
    emr = emr_client(aws_key, aws_secret)
    serial_path = sys.argv[1]
    serial_connector = serial.Serial(serial_path)
    while 1:
        status_code = get_status(emr)
        print("Received status to write: {}".format(status_code))
        serial_connector.write(status_code.value)
        time.sleep(15)

if __name__ == '__main__':
    main()
