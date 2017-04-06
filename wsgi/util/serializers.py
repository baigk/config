
import datetime

from lxml import etree
from oslo_log import log as logging
from oslo_serialization import jsonutils
import six

LOG = logging.getLogger(__name__)

class JSONRequestDeserializer(object):
    def is_json_content_type(self, request):
        content_type = request.content_type
        if not content_type or content_type.startswith('text/plain'):
            content_type = 'application/json'
        if (content_type in ('JSON', 'application/json') and request.body.startswith(b'{')):
            return True
        return False

    def has_body(self, request):
        if (int(request.content_length or 0) > 0 and
                self.is_json_content_type(request)):
            return True

        return False

    def from_json(self, datastring):
        try:
            return jsonutils.loads(datastring)
        except ValueError as ex:
            raise webob.exc.HTTPBadRequest(six.text_type(ex))

    def default(self, request):
        if self.has_body(request):
            return {'body': self.from_json(request.body)}
        else:
            return {}


class JSONResponseSerializer(object):

    def to_json(self, data):
        def sanitizer(obj):
            if isinstance(obj, datetime.datetime):
                return obj.isoformat()
            return six.text_type(obj)

        response = jsonutils.dumps(data, default=sanitizer)

        return response

    def default(self, response, result):
        response.content_type = 'application/json'
        response.body = six.b(self.to_json(result))


JSON_ONLY_KEYS = ('TemplateBody', 'Metadata')


class XMLResponseSerializer(object):

    def object_to_element(self, obj, element):
        if isinstance(obj, list):
            for item in obj:
                subelement = etree.SubElement(element, "member")
                self.object_to_element(item, subelement)
        elif isinstance(obj, dict):
            for key, value in obj.items():
                subelement = etree.SubElement(element, key)
                if key in JSON_ONLY_KEYS:
                    if value:
                        try:
                            subelement.text = jsonutils.dumps(value)
                        except TypeError:
                            subelement.text = str(value)
                else:
                    self.object_to_element(value, subelement)
        else:
            element.text = six.text_type(obj)

    def to_xml(self, data):
        root = next(six.iterkeys(data))
        eltree = etree.Element(root)
        self.object_to_element(data.get(root), eltree)
        response = etree.tostring(eltree)
        return response

    def default(self, response, result):
        response.content_type = 'application/xml'
        response.body = self.to_xml(result)
