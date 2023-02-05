"""
Deprecated: This module will not be supported in the future and should not be used for new projects. We recommend using intelhex instead: https://pypi.python.org/pypi/IntelHex.
"""

from builtins import int
import sys

py2 = sys.version_info[0] == 2
py3 = sys.version_info[0] == 3


class Segment(object):
    """Memory segment for use with API.write() method."""

    def __init__(self, addr, vals):
        self.address = addr
        self.data = vals
        self.length = len(vals)

    def append(self, vals):
        self.data.extend(vals)
        self.length += len(vals)


class Hex(object):
    """Parsed hex file."""

    def __init__(self, filename):

        self._segment_list = []
        self._high_address = 0

        with open(filename, 'r', 1) as file:
            
            high_address_changed = False
            for line in file:

                record_data_length, record_address, record_type, record_data, record_checksum = self._intel_hex_recored_parse(line)
                if record_type == "extended_linear_address_record":
                    self._high_address = record_data[0] * 0x1000000 + record_data[1] * 0x10000
                    high_address_changed = True # Set this to true so that a new segment is added for the next line
                elif record_type == "extended_segment_address_record":
                    self._high_address = ((record_data[0] << 8) | record_data[1]) * 16
                    high_address_changed = True
                elif record_type == "data_record":
                    # Check if segment_list is empty
                    if (len(self._segment_list) == 0 or high_address_changed):
                        self._segment_list.append(Segment(self._high_address + record_address, record_data))
                        high_address_changed = False # Set to false so next lines are check for appending
                    else:
                        # Check if data_record is the next address in current segment
                        current_segment = self._segment_list[-1]
                        record_full_address = self._high_address + record_address
                        if ( ( current_segment.address + current_segment.length ) == record_full_address ) :
                            # Append line to current segment
                            current_segment.append(record_data)
                        else:
                            # Add new segment
                            self._segment_list.append(Segment(self._high_address + record_address, record_data))
                else:
                    #implement more if desired or necessary
                    pass

    def _intel_hex_recored_parse(self, line):

        record_data_length = int(line[1:3], 16)

        record_address = int(line[3:7], 16)

        if line[7:9] == '00':
            record_type = "data_record"
        elif line[7:9] == '02':
            record_type = "extended_segment_address_record"
        elif line[7:9] == '04':
            record_type = "extended_linear_address_record"
        else:
            # implement more if desired or necessary
            record_type = "other"

        record_data = []
        for i in range(record_data_length):
            record_data.append(int(line[9+i*2:11+i*2], 16))

        record_checksum = int(line[9+record_data_length*2:11+record_data_length*2], 16)

        return record_data_length, record_address, record_type, record_data, record_checksum


    def __iter__(self):
        self._iter_index = 0
        return self

    # Define __next__() method for iterables only if we are running under python 3.
    if py3:
        def __next__(self):

            if self._iter_index == len(self._segment_list):
                raise StopIteration

            self._iter_index = self._iter_index + 1
            return self._segment_list[self._iter_index - 1]

    # Define next() method for iterables only if we are running under python 2.
    if py2:
        def next(self):

            if self._iter_index == len(self._segment_list):
                raise StopIteration

            self._iter_index = self._iter_index + 1
            return self._segment_list[self._iter_index - 1]
