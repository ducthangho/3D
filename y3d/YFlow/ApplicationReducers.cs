using Redux;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace YFlow
{
    public static class ApplicationReducers
    {
        public static ApplicationStates ReduceApplication(ApplicationStates previousState, IAction action)
        {
            return new ApplicationStates
            {
                isBusy = (action is SetBusyAction) ? ((SetBusyAction)action).isBusy : previousState.isBusy,
                ObjectManager = ObjectManagerComponent.Reducers.ObjectManagerReducer(previousState.ObjectManager, previousState.TestState, action),
                TestState = TestDetailComponent.Reducers.TestDetailReducer(previousState.TestState, action)
                //TestDetail = TestDetailReducer(previousState.TestDetail, action)
                //stepState = StepReducer(previousState,action)
            };
        }
    }
}
