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
        public static StepState EnterTestReducer(ApplicationState preState, EnterTest action)
        {
            //preState.CurrentTest = action.vtest;
            preState.CurrentTest = action.vtest;
            return preState.stepState;
        }

        public static StepState StepReducer(ApplicationState prevState, IAction action)
        {
            if (action is EnterTest)
                return EnterTestReducer(prevState, (EnterTest)action);

            return prevState.stepState;
        }


        public static ApplicationState ReduceApplication(ApplicationState previousState, IAction action)
        {
            return new ApplicationState
            {
                isBusy =  (action is SetBusyAction)? ((SetBusyAction)action).isBusy: previousState.isBusy,
                stepState = StepReducer(previousState,action)
            };
        }
    }
}
