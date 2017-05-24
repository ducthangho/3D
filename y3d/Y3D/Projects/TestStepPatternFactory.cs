using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Y3D.Projects
{
    class TestStepPatternFactory
    {
        public string PatternName { get; private set; }
        public string PatternStepType { get; private set; }
        public string SettingType { get; private set; }
        private readonly Type _type;
        
        private TestStepPatternFactory(Type type)
        {
            _type = type;

            using (var instance = CreateInstance())
            {
                PatternName = instance.PatternName;
                PatternStepType = instance.PatternStepType;
                SettingType = instance.SettingType;
            }
        }

        /// <summary>
        /// Creates the Notificaton Pattern user control.  
        /// Note this object can be safely cast to a INotificationPattern.
        /// </summary>
        /// <returns></returns>
        public TestStepPattern CreateInstance()
        {
            return (TestStepPattern)Activator.CreateInstance(_type);
        }

        public static List<TestStepPatternFactory> All()
        {
            return Assembly.GetExecutingAssembly().GetTypes()
                .Where(t => t.IsSubclassOf(typeof(TestStepPattern)))
                .Select(t => new TestStepPatternFactory(t))
                .ToList();
        }

        public static List<TestStepPatternFactory> AllLowSettings()
        {
            return Assembly.GetExecutingAssembly().GetTypes()
                .Where(t => t.IsSubclassOf(typeof(TestStepPattern)))
                .Select(t => new TestStepPatternFactory(t))
                .Where(t=> (t.PatternStepType=="lowpoly"))
                .ToList();
        }
    }
}
