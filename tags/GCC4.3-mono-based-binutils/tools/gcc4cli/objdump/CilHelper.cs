
using System;
using System.IO;
using System.Text;

using Mono.Cecil;
using Mono.Cecil.Metadata;

namespace gcc4cli.objdump {

    class CilHelper {

        static public byte[] GetCustomAttributeByteArray (CustomAttribute attr)
        {
            MemoryStream mstream = new MemoryStream ();
            BinaryWriter writer = new BinaryWriter (mstream);
            WriteCustomAttribute (writer, attr);
            writer.Close ();
            return mstream.ToArray ();
        }

        static void WriteCustomAttribute (BinaryWriter writer, CustomAttribute attr)
        {
            MethodReference ctor = attr.Constructor;

            writer.Write ((ushort)0x0001); // StdProlog

            for (int i = 0; i < ctor.Parameters.Count; i++) {
                TypeReference type = ctor.Parameters [i].ParameterType;
                object value = attr.ConstructorParameters [i];
                if (value is object []) {
                    object [] values = value as object [];
                    TypeReference t_val  = ((ArrayType) type).ElementType;
                    ElementType t_val_et = GetCorrespondingType (t_val);
                    writer.Write ((uint) values.Length);
                    for (int j = 0; j < values.Length; j++)
                        WriteCustomAttributeElem(writer, t_val_et, t_val, values [j]);
                } else {
                    WriteCustomAttributeElem(writer, GetCorrespondingType (type), type, value);
                }
            }

            if ((attr.Fields.Count + attr.Properties.Count) > 0)
                throw new NotSupportedException();

            writer.Write ((ushort) 0/* NumNamed */);
        }

        static void WriteCustomAttributeElem (BinaryWriter writer, ElementType etype, TypeReference type, object value)
        {
            switch (etype) {
            case ElementType.Boolean:
                writer.Write ((byte) ((bool) value ? 1: 0));
                break;
            case ElementType.Char:
                writer.Write ((ushort) (char) value);
                break;
            case ElementType.R4:
                writer.Write ((float) value);
                break;
            case ElementType.R8:
                writer.Write ((double) value);
                break;
            case ElementType.I1:
                writer.Write ((sbyte) value);
                break;
            case ElementType.I2:
                writer.Write ((short) value);
                break;
            case ElementType.I4:
                writer.Write ((int) value);
                break;
            case ElementType.I8:
                writer.Write ((long) value);
                break;
            case ElementType.U1:
                writer.Write ((byte) value);
                break;
            case ElementType.U2:
                writer.Write ((ushort) value);
                break;
            case ElementType.U4:
                writer.Write ((uint) value);
                break;
            case ElementType.U8:
                writer.Write ((long) value);
                break;
            case ElementType.String:
                if (value == null)
                    writer.Write ((byte) 0xff);
                else if ((value as string).Length == 0)
                    writer.Write ((byte) 0x00);
                else {
                    byte [] str = Encoding.UTF8.GetBytes (value as string);
                    WriteCompressedInteger (writer, (int) str.Length);
                    writer.Write (str);
                }
                break;
            case ElementType.Type:
                throw new NotImplementedException(etype.ToString());
            case ElementType.Object:
                // Boxed ValueType
                if (value == null) {
                    WriteCompressedInteger (writer, (int) ElementType.String);
                    writer.Write ((byte) 0xff);
                } else {
                    Type vt = value.GetType ();
                    etype = GetCorrespondingType (vt.Namespace + "." + vt.Name);
                    if (etype == ElementType.Object)
                        throw new NotImplementedException();
                    WriteCompressedInteger (writer, (int) etype);
                    WriteCustomAttributeElem (writer, etype, type, value);
                }
                break;
            case ElementType.Class:
                // an enum in fact
                etype = GetEnumElementType (value);
                WriteCustomAttributeElem (writer, etype, type, value);
                break;
            default:
                throw new NotImplementedException (etype.ToString());
            }
        }

        static ElementType GetEnumElementType (object value)
        {
            switch (Type.GetTypeCode (value.GetType ())) {
            case TypeCode.Byte:   return ElementType.U1;
            case TypeCode.SByte:  return ElementType.I1;
            case TypeCode.Int16:  return ElementType.I2;
            case TypeCode.UInt16: return ElementType.U2;
            case TypeCode.Int32:  return ElementType.I4;
            case TypeCode.UInt32: return ElementType.U4;
            case TypeCode.Int64:  return ElementType.I8;
            case TypeCode.UInt64: return ElementType.U8;
            default:
                throw new NotSupportedException ();
            }
        }

        static void WriteCompressedInteger (BinaryWriter writer, int value)
        {
            if (value < 0x80)
                writer.Write ((byte) value);
            else if (value < 0x4000) {
                writer.Write ((byte) (0x80 | (value >> 8)));
                writer.Write ((byte) (value & 0xff));
            } else {
                writer.Write ((byte) ((value >> 24) | 0xc0));
                writer.Write ((byte) ((value >> 16) & 0xff));
                writer.Write ((byte) ((value >> 8) & 0xff));
                writer.Write ((byte) (value & 0xff));
            }
        }

        static ElementType GetCorrespondingType (TypeReference type)
        {
            return GetCorrespondingType (type.FullName);
        }

        static ElementType GetCorrespondingType (string fullname)
        {
            switch (fullname) {
            case Constants.Boolean: return ElementType.Boolean;
            case Constants.Char:    return ElementType.Char;
            case Constants.SByte:   return ElementType.I1;
            case Constants.Int16:   return ElementType.I2;
            case Constants.Int32:   return ElementType.I4;
            case Constants.Int64:   return ElementType.I8;
            case Constants.Byte:    return ElementType.U1;
            case Constants.UInt16:  return ElementType.U2;
            case Constants.UInt32:  return ElementType.U4;
            case Constants.UInt64:  return ElementType.U8;
            case Constants.Single:  return ElementType.R4;
            case Constants.Double:  return ElementType.R8;
            case Constants.String:  return ElementType.String;
            case Constants.Type:    return ElementType.Type;
            case Constants.Object:  return ElementType.Object;
            default:                return ElementType.Class;
            }
        }

    }
}

