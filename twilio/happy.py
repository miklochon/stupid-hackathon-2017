# /usr/bin/env python
# Download the twilio-python library from http://twilio.com/docs/libraries
from twilio.rest import TwilioRestClient

# Find these values at https://twilio.com/user/account
account_sid = "AC73b9835b6166e6fb482a1a5f94fc99c9"
auth_token = ""
client = TwilioRestClient(account_sid, auth_token)

message = client.messages.create(to="+15712999961", from_="+15712483817", body="Worker #347-A resumed happy state.")
