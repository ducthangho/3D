using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Y3D.Flow.States;
using Y3D.Flow.Actions;
using Redux;

namespace Y3D.Flow
{
    public static class Reducers
    {

        public static TestDetailState EnterStepReducer(TestDetailState prevState, EnterStepAction action)
        {
            prevState.CurrentStep = action.stepName;
            return prevState;
        }

        public static TestDetailState TestDetailReducer(TestDetailState prevState, IAction action)
        {
            if (action is EnterStepAction)
            {
                EnterStepReducer(prevState, (EnterStepAction)action);
            }
            return prevState;
        }

        public static ObjectManagerState EnterTestReducer(ObjectManagerState prevState, EnterTestAction action)
        {
            prevState.CurrentTest = action.vtest;
            prevState.CurrentLayer = action.vtest.Oname + "_" + action.vtest.Id;

            if (prevState.TestInScence.ContainsKey(prevState.CurrentLayer))
            {
                prevState.TestInScence[prevState.CurrentLayer] = true;
            } else
            {
                prevState.TestInScence.Add(prevState.CurrentLayer, true);
            }
            return prevState;
        }
        public static ObjectManagerState ObjectManagerReducer(ObjectManagerState prevState, IAction action)
        {
            if (action is EnterTestAction)
            {
                EnterTestReducer(prevState, (EnterTestAction)action);
            }
            return prevState;
        }

        public static ApplicationState ReduceApplication(ApplicationState previousState, IAction action)
        {
            return new ApplicationState
            {
                isBusy =  (action is SetBusyAction)? ((SetBusyAction)action).isBusy: previousState.isBusy,
                TestDetail = TestDetailReducer(previousState.TestDetail,action)
                //stepState = StepReducer(previousState,action)
            };
        }
    }
}
