''' copyright (c) 2010 Fredrik Kihlander, see LICENSE for more info '''

config = {
		'header' : 
'''#if defined(_MSC_VER)

        typedef signed   __int8  int8_t;
        typedef signed   __int16 int16_t;
        typedef signed   __int32 int32_t;
        typedef signed   __int64 int64_t;
        typedef unsigned __int8  uint8_t;
        typedef unsigned __int16 uint16_t;
        typedef unsigned __int32 uint32_t;
        typedef unsigned __int64 uint64_t;

#elif defined(__GNUC__)
        #include <stdint.h>
#endif''',

		'array_names' : {
			'count' : 'count',
			'data'  : 'data'
		},
		
		'pod_names' : {
			'int8'   : 'int8_t',
			'int16'  : 'int16_t',
			'int32'  : 'int32_t',
			'int64'  : 'int64_t',
			'uint8'  : 'uint8_t',
			'uint16' : 'uint16_t',
			'uint32' : 'uint32_t',
			'uint64' : 'uint64_t',
			'fp32'   : 'float',
			'fp64'   : 'double'
		}
}

HEADER_TEMPLATE = '''#ifndef %(module)s_H_INCLUDED
#define %(module)s_H_INCLUDED

/*
	Autogenerated file for DL-type-library!
*/

%(header)s

%(user_code)s

#ifndef DL_ARRAY_CLASS_DEFINED
#define DL_ARRAY_CLASS_DEFINED

template<typename T>
struct TDLArray
{
	T* %(array_member_data)s;
	%%(uint32)s %(array_member_count)s;

	// TODO: Fix asserts so that user might specify them as they se fit!
	T& operator[](uint _Index)
	{
		// M_ASSERT(_Index < %(array_member_count)s, "Index out of range! Array size %%%%u, requested index %%%%u", %(array_member_count)s, _Index);
		return %(array_member_data)s[_Index];
	}

	const T& operator[](uint _Index) const 
	{
		// M_ASSERT(_Index < %(array_member_count)s, "Index out of range! Array size %%%%u, requested index %%%%u", %(array_member_count)s, _Index);
		return %(array_member_data)s[_Index];
	}
};

#endif // DL_ARRAY_CLASS_DEFINED 

'''

PODS = [ 'int8', 'int16', 'int32', 'int64', 'uint8', 'uint16', 'uint32', 'uint64', 'fp32', 'fp64', 'string', 'bitfield' ]
	
def temp_hash_func(str):
	hash = 5381
	for char in str:
		hash = (hash * 33) + ord(char)
	return (hash - 5381) & 0xFFFFFFFF;
	
class HeaderWriterCPP:
	def __write_member(self, str, attribs):
		if self.verbose:
			print >> self.stream, '\t// size %(size32)u, alignment %(align32)u, offset %(offset32)u' % attribs
			
		if 'comment' in attribs:
			print >> self.stream, (str + '\t// %(comment)s') % attribs
		else:
			print >> self.stream, str % attribs
			
	def write_header(self, data):
		print >> self.stream, HEADER_TEMPLATE % { 'module'    : data['module_name'].upper(), 
												  'user_code' : data.get('module_cpp_header', ''),
												  'header'    : config['header'],
												  'array_member_data'  : config['array_names']['data'],
												  'array_member_count' : config['array_names']['count'], }
		
	def finalize(self, data):
		print >> self.stream, '#endif // %s_H_INCLUDED' % data['module_name'].upper()
		
		# Replace stuff here!
		
		out_text = self.stream.getvalue()
		
		# print out_text
		
		self.out.write( out_text % config['pod_names'] )
		
	def write_enums(self, data):
		if data.has_key('module_enums'):
			for enum in data['module_enums'].items():
				enum_name = enum[0]
				enum_base = enum_name.upper()
				enum_values = enum[1]
				
				self.stream.write('enum ' + enum_name + '\n{')
				for i in range(0, len(enum_values)):
					value = enum_values[i]
					self.stream.write('\n\t%s_%s = %d' % (enum_base, value[0].upper(), value[1]))
					
					if i != len(enum_values) - 1:
						self.stream.write(',')
				print >> self.stream, '\n};\n'
	
	def create_header_order(self, outlist, type, data, enums):
		if type in outlist or type in PODS or type in enums:
			return
		
		member_types = []
		for m in data[type]['members']:
			m_type = m.get('subtype', m['type'])
			if m_type not in member_types and m_type != type:
				member_types.append(m_type)
		
		for m in member_types:
			self.create_header_order(outlist, m, data, enums)
			
		outlist.append(type)
	
	def write_structs(self, data):
		write_order = []
		
		mod_types  = data['module_types']
		enum_types = data.get('module_enums', {}).keys()
		
		for struct in mod_types.items():
			self.create_header_order(write_order, struct[0], mod_types, enum_types)
		
		for type_name in write_order:
			struct_name       = type_name
			struct_attrib     = mod_types[type_name]
			struct_orig_align = struct_attrib['original_align']
			
			if 'cpp-alias' in struct_attrib: continue # we have an alias so we force the user to provide the type.
			
			print >> self.stream, '// size ', struct_attrib['size32']
			
			if 'comment' in struct_attrib: print >> self.stream, '//', struct_attrib['comment']
			
			if struct_orig_align == 0 or struct_attrib['align'] > struct_orig_align:
				print >> self.stream, 'struct %s\n{' % struct_name
			else:
				print >> self.stream, 'struct DL_ALIGN( %u ) %s\n{' % ( struct_attrib['align'], struct_name )
			
			print >> self.stream, '\tconst static %%(uint32)s TYPE_ID = 0x%08X;\n' % temp_hash_func(struct_name)
			
			for member in struct_attrib['members']:
				type = member['type']
				if 'cpp-alias' in member: self.__write_member('\t%(cpp-alias)s %(name)s;',          member)
				elif type == 'bitfield':  self.__write_member('\t%%(%(subtype)s)s %(name)s : %(bits)u;', member)
				elif type == 'string':    self.__write_member('\tconst char* %(name)s;',            member)
				elif type == 'pointer':   self.__write_member('\tconst %(subtype)s* %(name)s;',     member)
				elif type in PODS:        self.__write_member('\t%%(%(type)s)s %(name)s;',               member)
				elif type in enum_types:  self.__write_member('\t%(type)s %(name)s;',               member)
				elif type == 'inline-array':
					subtype = member['subtype']
					if subtype == 'string': self.__write_member('\tchar* %(name)s[%(count)u];',       member)
					elif subtype in PODS:   self.__write_member('\t%%(%(subtype)s)s %(name)s[%(count)u];', member)
					else:                   self.__write_member('\t%(subtype)s %(name)s[%(count)u];', member)
				elif type == 'array':
					subtype = member['subtype']
					if subtype == 'string': self.__write_member('\tTDLArray<char*> %(name)s;',       member)
					elif subtype in PODS:   self.__write_member('\tTDLArray<%%(%(subtype)s)s> %(name)s;', member)
					else:                   self.__write_member('\tTDLArray<%(subtype)s> %(name)s;', member)
				elif 'cpp-alias' in mod_types[type]:
					self.__write_member('\t%s %(name)s;' % mod_types[type]['cpp-alias'], member)
				else:
					self.__write_member('\t%(type)s %(name)s;', member)
			
			print >> self.stream, '};\n'
		
	def __init__(self, out):
		from StringIO import StringIO
		self.stream  = StringIO()
		self.out     = out
		self.verbose = False
