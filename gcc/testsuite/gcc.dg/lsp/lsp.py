# A python module for implementing LSP clients

import json

import jsonrpc # pip install json-rpc
import requests

# Various types to model the LSP interface

class Position:
    def __init__(self, line, character):
        self.line = line
        self.character = character

    def __repr__(self):
        return 'Position(%r, %r)' % (self.line, self.character)

    def __eq__(self, other):
        return self.__dict__ == other.__dict__

    def to_json(self):
        return {'line': self.line, 'character': self.character}

    @staticmethod
    def from_json(js):
        return Position(js['line'], js['character'])

class Range:
    def __init__(self, start, end):
        self.start = start
        self.end = end

    def __repr__(self):
        return 'Range(%r, %r)' % (self.start, self.end)

    def __eq__(self, other):
        return self.__dict__ == other.__dict__

    @staticmethod
    def from_json(js):
        return Range(Position.from_json(js['start']),
                     Position.from_json(js['end']))

class Location:
    def __init__(self, uri, range):
        self.uri = uri
        self.range = range

    def __repr__(self):
        return 'Location(%r, %r)' % (self.uri, self.range)

    def __eq__(self, other):
        return self.__dict__ == other.__dict__

    @staticmethod
    def from_json(js):
        return Location(js['uri'], Range.from_json(js['range']))

    def dump(self, msg):
        print('%s:%i:%i: %s' % (self.uri, self.range.start.line,
                                self.range.start.character, msg))
        # FIXME: underline
        # linecache uses 1-based line numbers, whereas LSP uses
        # 0-based line numbers
        import linecache
        line = linecache.getline(self.uri, self.range.start.line + 1)
        print('line: %r' % line)

class TextDocumentIdentifier:
    def __init__(self, uri):
        self.uri = uri

    def __eq__(self, other):
        return self.__dict__ == other.__dict__

    def to_json(self):
        return {'uri': self.uri}

class TextDocumentPositionParams:
    def __init__(self, textDocument, position):
        self.textDocument = textDocument
        self.position = position

    def __eq__(self, other):
        return self.__dict__ == other.__dict__

    def to_json(self):
        return {"textDocument" : self.textDocument.to_json(),
                "position" : self.position.to_json()}

# A wrapper for making LSP calls against a server

class Proxy:
    def __init__(self, url):
        self.url = url
        self.next_id = 0

    def make_request(self, method, params):
        json_req = {"method": method,
                    "params": params,
                    "jsonrpc": "2.0",
                    "id": self.next_id}
        self.next_id += 1
        return json_req

    def post_request(self, method, params):
        payload = self.make_request(method, params)
        headers = {'content-type': 'application/json'}
        response = requests.post(self.url, data=json.dumps(payload),
                                 headers=headers)
        print('response: %r' % response)
        print('response.json(): %r' % response.json())
        return response.json()

    def goto_definition(self, textDocument, position):
        params = TextDocumentPositionParams(textDocument, position)
        json_resp = self.post_request('textDocument/definition',
                                      params.to_json())
        print(json_resp)
        # Expect either a Location or a list of Location
        if isinstance(json_resp, list):
            return [Location.from_json(item) for item in json_resp]
        else:
            return Location.from_json(json_resp)

