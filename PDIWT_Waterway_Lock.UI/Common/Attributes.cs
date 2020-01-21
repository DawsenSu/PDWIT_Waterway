using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PDIWT
{
    /// <summary>
    /// ECProperty: mark poco property according to ECProperty defined in ECSchema
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class ECPropertyAttribute : Attribute
    {
        public ECPropertyAttribute(string accessstring)
        {
            AccessString = accessstring;
            //Scale = scale;
        }
        /// <summary>
        /// The EC property AccessString.
        /// </summary>
        public string AccessString { get; private set; }
        /// <summary>
        /// The ratio of ECProperty to POCO class property, scale = ECProperty value / POCO class Property value
        /// </summary>
        //public double Scale { get; private set; }
        //public Type PropertyType { get; set; }
    }
    /// <summary>
    /// ECClass: mark poco class according to ECClass defined in ECSchema
    /// </summary>
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct, AllowMultiple = false, Inherited = false)]
    public class ECClassAttribute: Attribute
    {
        public ECClassAttribute(string schemaname, string classname)
        {
            SchemaName = schemaname;
            ClassName = classname;
        }
        public string SchemaName { get; private set; }
        public string ClassName { get; private set; }
    }

    public enum Units
    {
        Meter,
        Centimeter,
        Millimeter
    }

    /// <summary>
    /// Identify the unit of this property.
    /// </summary>
    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class UnitScaleAttribute : Attribute
    {
        public UnitScaleAttribute(Units unit)
        {
            Unit = unit;
        }
        /// <summary>
        /// Mark the Unit of this property
        /// </summary>
        public Units Unit { get; private set; }


        public double ToMeterScale()
        {
            switch (Unit)
            {
                case Units.Meter:
                    return 1;
                case Units.Centimeter:
                    return 0.1;
                case Units.Millimeter:
                    return 0.001;
                default:
                    return 1;
            }
        }

        public double ToMillimeterScale()
        {
            switch (Unit)
            {
                case Units.Meter:
                    return 1000;
                case Units.Centimeter:
                    return 10;
                case Units.Millimeter:
                    return 1;
                default:
                    return 1;
            }
        }
    }
}
