from __future__ import annotations
from xml.etree import ElementTree
from pathlib import Path
from typing import Optional, Union


class XmlProcessError(Exception):
    def __init__(
        self,
        *args,
        xml_elem: ElementTree.Element,
        xml_file_path: Optional[Path] = None,
    ):
        super().__init__(*args)
        self.xml_elem = xml_elem
        self.xml_file_path = xml_file_path

    def set_xml_file_path(self, xml_file_path: Path):
        if self.xml_file_path is not None:
            raise Exception("xml_file_path is already set")
        self.xml_file_path = xml_file_path

    def set_xml_file_path_if_missing(self, xml_file_path: Path):
        if self.xml_file_path is None:
            self.xml_file_path = xml_file_path

    def __str__(self):
        message = super().__str__() + " on element: " + str(self.xml_elem)
        try:
            elem_str = ElementTree.tostring(self.xml_elem, encoding="unicode")
        except:
            import traceback

            message += traceback.format_exc()
        else:
            message += " " + elem_str.strip()
        if self.xml_file_path is not None:
            message += f" (in {self.xml_file_path})"
        return message


def xml_check_tag(elem: ElementTree.Element, valid_tags: Union[str, set[str]]):
    if isinstance(valid_tags, str):
        tag = valid_tags
        if elem.tag != tag:
            raise XmlProcessError(
                f"Element tag is {elem.tag!r} instead of the expected {tag!r}",
                xml_elem=elem,
            )
    else:
        if elem.tag not in valid_tags:
            valid_tags_str = ", ".join(map(repr, valid_tags))
            raise XmlProcessError(
                f"Element tag is {elem.tag!r} instead of one of the expected {valid_tags_str}",
                xml_elem=elem,
            )


GLOBALLY_VALID_ATTRIBUTES = {
    # TODO the HACK_IS_STATIC_ON hack is really something...
    # note Static isn't globally valid, only on resources besides Symbol (iirc)
    # but whatever
    "Static",
}


def xml_check_attributes(
    elem: ElementTree.Element,
    required_attributes: set[str],
    optional_attributes: set[str],
):
    attributes = elem.attrib.keys()

    if not required_attributes.issubset(attributes):
        missing_attributes = required_attributes - attributes
        raise XmlProcessError(
            f"{len(missing_attributes)} missing attribute(s)",
            missing_attributes,
            xml_elem=elem,
        )

    all_known_attributes = (
        GLOBALLY_VALID_ATTRIBUTES | required_attributes | optional_attributes
    )
    if not all_known_attributes.issuperset(attributes):
        unknown_attributes = attributes - all_known_attributes
        raise XmlProcessError(
            f"{len(unknown_attributes)} unknown attribute(s)",
            unknown_attributes,
            xml_elem=elem,
        )
