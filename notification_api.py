import os

from flask import Flask
from emr_status import emr_client
from emr_status import get_status

application = Flask(__name__)

aws_key = os.environ['AWS_KEY']
aws_secret = os.environ['AWS_SECRET']


@application.route('/status')
def status():
    emr = emr_client(aws_key, aws_secret)
    status_code = get_status(emr)
    return status_code.value
