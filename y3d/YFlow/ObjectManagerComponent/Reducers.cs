using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using y3d.e;
using Redux;

namespace YFlow.ObjectManagerComponent
{
    public static class Reducers
    {
        public static States EnterTestReducer(States prevState, EnterTestAction action)
        {
            prevState.CurrentTest = action.vtest;
            prevState.CurrentLayer = action.vtest.Oname + "_" + action.vtest.Id;

            if (!prevState.TestInScence.ContainsKey(prevState.CurrentLayer))
            {
                prevState.TestInScence.Add(prevState.CurrentLayer, false);
            }
                //if (prevState.TestInScence.ContainsKey(prevState.CurrentLayer))
                //{
                //    prevState.TestInScence[prevState.CurrentLayer] = true;
                //}
                //else
                //{
                //    prevState.TestInScence.Add(prevState.CurrentLayer, true);
                //}
             return prevState;
        }

        public static States ExitTestReducer(States prevState, YFlow.TestDetailComponent.States testState, ExitTestAction action)
        {
            if (testState.TestStepState == TestDetailComponent.EditStateEnum.Ready)
            {
                prevState.CurrentTest = null;
                prevState.CurrentLayer = "0";
            }
            return prevState;
        }

        public static States AddTestReducer(States prevState, AddTestAction action)
        {
            if (prevState.TestData == null) prevState.TestData = new UserTestData();
            if (!prevState.TestData.Utests.ContainsKey(action.vtest.Oname))
            {
                prevState.TestData.Utests.Add(action.vtest.Oname, new YListTest());
            }
            prevState.TestData.Utests[action.vtest.Oname].Otests.Add(action.vtest);
            prevState.ListTest = prevState.TestData.Utests[action.vtest.Oname].Otests;
            prevState.isSaved = false;

            prevState.CurrentTest = action.vtest;
            prevState.CurrentLayer = action.vtest.Oname + "_" + action.vtest.Id;

            if (!prevState.TestInScence.ContainsKey(prevState.CurrentLayer))
            {
                prevState.TestInScence.Add(prevState.CurrentLayer, true);
            } else
            {
                prevState.TestInScence[prevState.CurrentLayer] = true;
            }
            return prevState;
        }

        public static States DeleteTestReducer(States prevState, DeleteTestAction action)
        {
            if (prevState.TestData == null) prevState.TestData = new UserTestData();
            if (!prevState.TestData.Utests.ContainsKey(action.vtest.Oname))
            {
                prevState.TestData.Utests.Add(action.vtest.Oname, new YListTest());
            }
            prevState.TestData.Utests[action.vtest.Oname].Otests.Remove(action.vtest);
            prevState.ListTest = prevState.TestData.Utests[action.vtest.Oname].Otests;
            prevState.TestInScence.Remove(action.vtest.Oname + "_" + action.vtest.Id);
            prevState.CurrentLayer = "0";
            prevState.CurrentTest = null;
            prevState.isSaved = false;
            return prevState;
        }

        public static States SaveTestReducer(States prevState, SaveTestAction action)
        {
            prevState.isSaved = true;
            return prevState;
        }

        public static States SelectObjectReducer(States prevState, SelectObjectAction action)
        {
            var aa = prevState.CurrentYAL.Areas;
            foreach (var ya in aa)
            {
                var co = ya.Objs.FirstOrDefault(x => x.Name == action.oname);
                if (co != null)
                {
                    prevState.CurrentObject = co;
                    if (prevState.TestData != null)
                    {
                        if (prevState.TestData.Utests != null)
                        {
                            if (prevState.TestData.Utests.ContainsKey(prevState.CurrentObject.Name))
                            {
                                prevState.ListTest = prevState.TestData.Utests[action.oname].Otests;
                            }
                            else
                            {
                                prevState.ListTest = null;
                            }
                        }
                        else
                        {
                            prevState.ListTest = null;
                        }
                    }
                    return prevState;
                }
            }
            prevState.CurrentObject = null;
            prevState.ListTest = null;
            return prevState;
        }
        public static States ResetYAreaReducer(States prevState, ResetYAreaAction action)
        {
            prevState.CurrentObject = null;
            prevState.CurrentYAL.Areas.Clear();
            return prevState;
        }

        public static States SetYAreaReducer(States prevState, SetYAreaAction action)
        {
            //prevState.CurrentObject = null;
            prevState.CurrentYAL = action.yal;
            return prevState;
        }

        public static States ObjectManagerReducer(States prevState, YFlow.TestDetailComponent.States testState, IAction action)
        {
            if (action is EnterTestAction)
            {
                return EnterTestReducer(prevState, (EnterTestAction)action);
            }
            else if (action is AddTestAction)
            {
                return AddTestReducer(prevState, (AddTestAction)action);
            }
            else if (action is DeleteTestAction)
            {
                return DeleteTestReducer(prevState, (DeleteTestAction)action);
            }
            else if (action is ExitTestAction)
            {
                return ExitTestReducer(prevState, testState, (ExitTestAction)action);
            }
            else if (action is SaveTestAction)
            {
                return SaveTestReducer(prevState, (SaveTestAction)action);
            }
            else if (action is ResetYAreaAction)
            {
                return ResetYAreaReducer(prevState, (ResetYAreaAction)action);
            }
            else if (action is SetYAreaAction)
            {
                return SetYAreaReducer(prevState, (SetYAreaAction)action);
            }
            else if (action is SelectObjectAction)
            {
                return SelectObjectReducer(prevState, (SelectObjectAction)action);
            }
            return prevState;
        }

    }
}
