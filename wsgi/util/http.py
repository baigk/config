from six.moves import urllib
import requests
import collections
import logging
import base64
import json
import os
import six

class ACLDisabled(Exception):
    pass


class ACLPermissionDenied(Exception):
    pass


class NotFound(Exception):
    pass


class Timeout(Exception):
    pass


class BadRequest(Exception):
    pass

Response = collections.namedtuple('Response', ['code', 'headers', 'body'])

class callback(object):
    @classmethod
    def __status(cls, response):
        # status checking
        if response.code >= 500 and response.code < 600:
            raise Exception("%d %s" % (response.code, response.body))
        if response.code == 400:
            raise BadRequest('%d %s' % (response.code, response.body))
        if response.code == 402:
            raise ACLDisabled(response.body)
        if response.code == 403:
            raise ACLPermissionDenied(response.body)
        if response.code == 404:
            raise NotFound(response.body)

    @classmethod
    def bool(cls):
        def cb(response):
            callback.__status(response)
            return response.code == 200
        return cb

    @classmethod
    def json(cls, map=None):
        def cb(response):
            callback.__status(response)
            if response.code == 404:
                return None

            return json.loads(response.body)

        return cb

    @classmethod
    def null(cls, map=None):
        def cb(response):
            callback.__status(response)
            if response.code == 404:
                return None

            return response.body
        return cb

class HTTPClient(object):
    def __init__(self, host='127.0.0.1', port=8500, scheme='http', verify=True):
        self.host = host
        self.port = port
        self.scheme = scheme
        self.verify = verify
        self.base_uri = '%s://%s:%s' % (self.scheme, self.host, self.port)
        self.session = requests.session()

    def response(self, response):
        response.encoding = 'utf-8'
        return Response(response.status_code, response.headers, response.text)

    def uri(self, path, params=None):
        uri = self.base_uri + path
        if not params:
            return uri
        return '%s?%s' % (uri, urllib.parse.urlencode(params))

    def get(self, callback, path, params=None):
        uri = self.uri(path, params)
        return callback(self.response(
            self.session.get(uri, verify=self.verify)))

    def put(self, callback, path, params=None, data=''):
        uri = self.uri(path, params)
        return callback(self.response(
            self.session.put(uri, data=data, verify=self.verify)))

    def delete(self, callback, path, params=None):
        uri = self.uri(path, params)
        return callback(self.response(
            self.session.delete(uri, verify=self.verify)))

    def post(self, callback, path, params=None, data='', json=None):
        uri = self.uri(path, params)
        return callback(self.response(
            self.session.post(uri, data=data, json=json, verify=self.verify)))

