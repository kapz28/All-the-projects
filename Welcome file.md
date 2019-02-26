# GeoScenario 1.1

GeoScenario is the format used to represent scenarios.
It is based on the OpenStreetMap format; every GeoScenario document is a valid `.osm` document, using custom properties to define scenario-specific information.

## Nodes

A _node_ is a single LLA point. GeoScenario defines the following presets for nodes:

### Global Config

Global configurations for a scenario. Defines a name for a scenario, the road network (lanelet), and global failure conditions (timeout and/or collision).

- **Version** (`version`): will indicate the GeoScenario version of the file 
 - **Name** (`name`): The name given to this scenario.
- **Lanelet** (`lanelet`): Lanelet file which represents the Road Network
- **Collision** (`collision`): If `yes` scenario will fail when a collision happens with Ego .., If `no` scenario will not fail when a collision happens with Ego 
- **Timeout** (`timeout`): The amount of seconds that will cause the scenario to fail, if the Ego has not reached it's goal yet within that time
- **Metrics**(`metrics`): Metrics to be collected in this scenario Ex: _*"ttc_ego_v1, ttc_ego_v2"*_

### Ego Start

Indicates the spawn location of the ego. Only one of these should exist per scenario. The ego will be spawned at the location of the first one, with the provided rotation.

- **Name** (`name`): The name given to this spawn location.
- **Orientation** (`orientation`): The heading that the ego should be spawned at, in degrees clockwise from East.
- **Speed**: _Currently not implemented_
- **Path ID**: _Currently not implemented_

### Ego Goal

Indicates a goal point for the ego. Multiple goals can exist per scenario. At runtime, a trigger with radius 2 m will be spawned at the goal point, and will end the scenario as soon as the ego reaches it.

- **Name** (`name`): The name given to this goal point.
- **Order** (`order`): Integer that can be used as an unordered goal which is represented by only a value of 0 or can be used as an ordered goal which can exclusively one of any value {1,2,3,4,5,..}. 

Ordered goals are used when multiple goals must be reached in a certain order. Any unordered goal is interpreted as sufficient condition for termination of a scenario with success state. If order and unordered are available, ego passes the scenario if at least one unordered goal is reached, or if all ordered goals are reached in the right order.

### Vehicle

Indicates that a scenario vehicle should be spawned at the provided node.

- **Name** (`name`): The name assigned to this vehicle.
- **Dynamic Interaction Levels** (`dynamicinteractionlevels`): The Dynamic Interaction levels at which this vehicle will spawn. The provided value for this field must be a "list of ranges". The format for a list of ranges is indicated below.
- **Orientation** (`orientation`): The heading that the vehicle should be spawned at, in degrees clockwise from East.
- **Speed** (`speed`): The default speed that this vehicle will drive at, in km/h.
- **Use Speed Profile** (`usespeedprofile`): `true` if this vehicle should follow the speed profile of its specified path; `false` if it should drive at a constant default speed.
- **Path ID** (`path`): The name of the path that this vehicle is assigned to.
- **Start In Motion** (`startinmotion`): `true` if this vehicle should begin moving immediately when the scenario starts; `false` if it should remain stationary. This property will have no effect unless the vehicle has a path assigned.
- **Cycle Limit** (`cyclelimit`): The maximum number of times that a vehicle will complete its full path. If set to -1, the vehicle will repeat its path forever.
- **Loop** (`loop`): `true` if the vehicle should treat its assigned path as a closed loop. If `false`, the vehicle will teleport back to the first point once it reaches the last point, assuming that it has not yet reached its cycle limit. If `true`, the vehicle will drive from the last point back to the first point instead.
- **Driver**: _Currently not implemented_

### Pedestrian

Indicates that a scenario pedestrian should be spawned at the provided node.

- **Name** (`name`): The name assigned to this pedestrian.
- **Dynamic Interaction Levels** (`dynamicinteractionlevels`): The Dynamic Interaction levels at which this pedestrian will spawn. The provided value for this field must be a "list of ranges". The format for a list of ranges is indicated below.
- **Orientation** (`orientation`): The heading that the pedestrian should be spawned at, in degrees clockwise from East.
- **Speed** (`speed`): The default speed that this pedestrian will drive at, in km/h.
- **Use Speed Profile** (`usespeedprofile`): `true` if this pedestrian should follow the speed profile of its specified path; `false` if it should drive at a constant default speed.
- **Path ID** (`path`): The name of the path that this pedestrian is assigned to.
- **Start In Motion** (`startinmotion`): `true` if this pedestrian should begin moving immediately when the scenario starts; `false` if it should remain stationary. This property will have no effect unless the pedestrian has a path assigned.
- **Cycle Limit** (`cyclelimit`): The maximum number of times that a pedestrian will complete its full path. If set to -1, the pedestrian will repeat its path forever.
- **Loop** (`loop`): `true` if the pedestrian should treat its assigned path as a closed loop. If `false`, the pedestrian will teleport back to the first point once it reaches the last point, assuming that it has not yet reached its cycle limit. If `true`, the pedestrian will drive from the last point back to the first point instead.

### Static Object

Indicates that a static object should be spawned at the provided node.

- **Name** (`name`): The name assigned to this static object.
- **Type** (`type`): The type assigned to this static object. This makes no difference in the simulation, it is simply contextual information about what the object represents in the scenario.
- **Shape** (`shape`): The shape mesh used by the object. This must be one of "Cube", "Cone", "Cylinder", "Sphere", or "Plane" (case insensitive).
- **Static Reaction Levels** (`staticreactionlevels`): The Static Reaction levels at which this static object will spawn. The provided value for this field must be a "list of ranges". The format for a list of ranges is indicated below.
- **Elevation** (`elevation`): How far off the ground (in centimetres) the object should be placed.
- **Scale X** (`scalex`): The scale of the object in the x direction.
- **Scale Y** (`scaley`): The scale of the object in the y direction.
- **Scale Z** (`scalez`): The scale of the object in the z direction.
- **Roll** (`roll`): The rotation of the object along the roll axis.
- **Pitch** (`pitch`): The rotation of the object along the pitch axis.
- **Yaw** (`yaw`): The rotation of the object along the yaw axis.
- **Simulate Physics** (`simulatephysics`): `"yes"` to indicate that the object should have simulated physics, and `"no"` if not. Static objects with simulated physics have gravity and rotational physics, allowing them to fall, roll, and interact with the environment. Static objects also interact and collide with each other.
- **Area** (`area`): If `"yes"` this object will be interpreted as a polygon following the shape of the way forming an area. This is only true if the last and first nodes of the way are pointing to the same node., and `"no"` means the way indicates the object boundaries for it's dimensions (maximum width and length). However, the object still keeps its natural shape. Note when object is defined as single node attribute is always `"no"`
- **Height** (`height`): The object's height in cm
- **Orientation** (`orientation`): Object's orientation in degrees

### Scenario Trigger

Indicates that a scenario trigger should be spawned at the given node. A trigger may have one or more _actions_ assigned to it. Each of these actions will be carried out once, when the specified trigger conditions are met.

- **Name** (`name`): The name assigned to this trigger.
- **Actor Activate** (`actor`): The name of the actor that will activate this trigger when it overlaps the trigger volume.
- **Actor Target** (`target`): The name of the actor that will be affected by the trigger action.
- **Action Delay** (`actiondelay`): The delay in seconds between the actor overlapping the trigger and the action being executed.
- **Change Speed** (`actionspeed`): If present, the target actor will change to this speed in km/h when the action is executed.
- **Spawn**: _Currently not implemented_
- **Start** (`actionstart`): If present and `true`, the target actor will start moving (if a path is assigned) when the action is executed.
- **Follow Path** (`actionpath`): If present, the specified path will be assigned to the target actor when the action is executed.

### Path node

The use of path nodes is explained in the next section "Ways"

- **Target speed of agent (km/h)** (`agentspeed`) - the velocity that the agent should have when it reaches this path node.
- **Target acceleration of agent (g)** (`agentacceleration`) - the acceleration the agent should use to achieve the velocity at the next node.
- **Time to reach target acceleration of agent (s)** (`timetoacceleration`) - the time in which an agent should reach the given acceleration. The acceleration is linearly increased from the agent's previous acceleration to the target one over this period of time.

### Origin point

Origin point, used as a reference point to translate all coordinates into the simulation tool coordinate system.

- **Name** (`name`): The name given to this spawn location.
- **Orientation** (`orientation`): The heading that the ego should be spawned at, in degrees clockwise from East.

### Metric

Defines a metric that needs to be tracked during the scenario.

- **Name** (`name`): The name given to this spawn location.
- **Orientation** (`orientation`): The heading that the ego should be spawned at, in degrees clockwise from East.

### Traffic Light

Traffic light represented as a stationary element  and is part of the road network

- **Name** (`name`): The name given to this spawn location.
- **Orientation** (`orientation`): The heading that the ego should be spawned at, in degrees clockwise from East.

### Location

location point, which is used as a reference point for other elements within a scenario. For example, an area where a pedestrian will be placed.

- **Name** (`name`): The name given to this spawn location.
- **Orientation** (`orientation`): The heading that the ego should be spawned at, in degrees clockwise from East.

### Spawn

_Currently not implemented_

## Ways

A _way_ is an ordered collection of nodes, typically denoting a path, route, or boundary.
GeoScenario defines two presets for ways:

### Vehicle Path

Indicates that this way represents a path to be followed by a vehicle. Although JOSM connects nodes in ways with straight lines, this path will be converted into a spline when parsed by Unreal.

By default, a scenario agent will travel along a path at a constant speed.
In order to specify a speed profile for an agent, that agent must have the **Use Speed Profile** (`usespeedprofile`) tag set to `true`, and be assigned to a path where every node has a nonzero `agentspeed` tag.
The `agentspeed` indicates the speed in km/h that the agent will be travelling at once it reaches that node.
Each node can also have an `agentacceleration` and a `timetoacceleration` tag.
The `agentacceleration` tag specifies the target acceleration in units of `g` (9.81 m/s) for the agent at that node, and this target acceleration will be reached in a duration of time after it hits this node, specified by the `timetoacceleration` tag, in units of seconds.
The acceleration will change linearly for this entire duration, but if it hits the next node before it reaches this target acceleration, it will move on to the next target.

The agent will always try to match the speed of the next node in its path, but it will only try to match the acceleration of the node that it has most recently passed.

If you do not include the 2 acceleration tags, the agent's speed will vary with a constant acceleration dependent on the length of the path section and the starting and ending speeds.

- **Name** (`name`): The name assigned to this path.

### Pedestrian Path

Currently, there is no difference between a vehicle and pedestrian path.

- **Name** (`name`): The name assigned to this path.

#### List of Ranges Format

The **Static Reaction** and **Dynamic Interaction** level tags take a "list of ranges" as their value format. A list of ranges is a _comma-separated list of either numbers of "ranges"_.

A range is of the form `a..b`, where `a` and `b` are either the empty-string, or integers.
- `a..b` indicates the levels `a` to `b` (inclusive).
- `..b` indicates the levels 0 to `b`.
- `a..` indicates the levels `a` to 5 (with 5 being the maximum static reaction level).
- `..` indicates levels 0 to 5.

Examples of valid "lists of ranges" include `"4"`, `"1..3"`, `"2.."`, `"0, 3, 5"`, `"1, 4.."`, `"..2,    4..5"`, `"..,1,2,3"`. It doesn't matter if values are repeated.

Examples of invalid lists of ranges include:
- `"6"` - issue: level is too high, levels are clamped between 0 and 5. The GeoScenarioParser will warn that this will be converted to the max value of `5`.
- `"a"` - issue: unable to convert `"a"` to an integer. The GeoScenarioParser will log this as an error, and skip parsing this range.
- `"1.5"` - issue: unable to convert `"1.5"` to an integer, _or_ invalid range format (expected two `.`'s). The GeoScenarioParser will log this as an error, and skip parsing this range.
- `"1...4"` - issue: invalid range format (expected two `.`'s). The GeoScenarioParser will log this as an error, and skip parsing this range.
- `"4..3"` - issue: invalid range format, lower bound is greater than upper bound. The GeoScenarioParser will log this as an error, and skip parsing this range.

If after parsing all ranges, none of them were valid format, the parser will default to assuming _all levels_ (`".."`).

**Note:** `"3..3"` is valid, and will simply be parsed as `3`. `"..0"` and `"5.."` are also valid.

### Location

location point, which is used as a reference point for other elements within a scenario. For example, an area where a pedestrian will be placed.

- **Name** (`name`): The name given to this spawn location.
- **Orientation** (`orientation`): The heading that the ego should be spawned at, in degrees clockwise from East.

## The Road Network
Lanelets are used to represent the scenario's Road Network. The Road Network is stored in a separate XML file to make replacements easy. However, a scenario can only be interpreted within the context of the Road Network. Thus, a Geoscenario must always be distributed with its associated Road Network file. The Lanelet file is defined inside the Global Config element.

  

![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAgEAAACyCAYAAADbJxyTAAA7oElEQVR4nO2dCXhTVfrGT9KWslPKWkCKgAs7rqi4dEQFN9QRHZ2/MiJtKXRQRkdnxmXsuIzIOAwubC0IiisKOCC4iwooroMLIiIKAgXK0rLTLfm/b869EtKkTdskNzf5fs/z9S65ufe7aXLPe875zncSlSAIghCIY2C/g70OW22xLybXw3JhjWEzYZOtdUewM4lWOyAIghClpMOWKS0Eboc9AptkpUNKP7P/ButjbP8Z9hysxCqHBHsjIkAQBME/DystAEh72BXKehHQCpbitd0adiJspSXeCLZHRIAgCIJ/PlW66d1kp1WOeLEd9o3T6WzvdrsTYF8rEQBCPRARIAiC4J+CpKSkMxs3bnz1nj173lW6Hz4auOykk05aeBisXr36RqudEeyNiABBEAT/HBg5cuS/XS5X9/z8/MFWO+OF+5RTTtnkdru/hQg4bLUzgr0RESAIghAAFLSdsfjFaj98gV9NnE7nfqv9EOyPiABBiCyM7Gat8lHYa3V4fzPYVYY5YFca+xOM10qqeS+bs6+B5cOer8O1reZe2CDYeNgbEbpmVIoAh8PR1OVyHbDaD8H+iAgQhMjSA3ae0sO66sKbsDONde9Atemwp2AfVfPebsa136zjta2mp9L+z47UBVHjTkeBuzFS1wsW+NQEvklLgFBvRAQIgn3gMDUKADfsL0onsCEPwUYqLQKE0MKWgGVWO+GLy+VqCiEgLQFCvRERIAj2obWxXA/7l9f+IRb4Ei90RmEbjd0B0hIghAQRAYIQPbCff6jSyWBWwZ6A7TZe6wJ70Fhvp3ST+D7YXuM1cjdsqdLxBjXBuIRrlU49+z6sAObyOYZN75cqnYxmDWwRbLnX6x1g/4QVwe702k//J8FYSP3R2HcR7Pew92AbYDcY9/Eh7ElYqc+1myodwzAAVgibFcQ9hYN0p9MZTSKAsR+Pz58//8QDBw7chvXRMGkREOqMiABBiA5eUrpQNrlS6Qc8C08mhGkJO994rYnx+k7DWhj7z1VVC1N/ZMGO9dq+DnYhbJjXPgYwspvBYWxfDrsDdpfSgXkkBfYH2E/qaBHQ2NhPAWOKgF7GPvrIJvYEYz9FD4MVz/Pyne+n2Ojndc5spcVGxMjOzqYfTadMmbI9ktetAX4nsnbu3JmEJXMEFMNutdYlwc6ICBAE6xmhtADYApsAYyAaa3ksMBlA2B/2P9hZsG+ULnSP83r/F7CTla7dVxcYaEIB8Jhx7NmwsbCrlY43+FjpALz7lRYATJLzotKT6FwAewA2H/ZD3W7Vc+2lxn1R1FCAsLZPYfKkcQwLNwqActg/YD8qLRSuruM16wrFyialYzCihQxYktd2X4v8EGIEEQGCYD0jjCUL3nxjnalgN8N6w06AfRfC6zHQbZyxPhd2qtICgEKCIuC3Sj8bnlG69k5mwJ6F/Z/S3RaP1PHaO5Ru3ahQegY85sKnePEuzE42lnz9IWOdwmOrcXykiMZ4gGfh0yC3252C9UNKCzRBqDMiAgTBerp5Lf/qtZ+FHiewCbUIWOOz/b3SIsDsVuhgLH1bFSgQ/k8dmVSnLixRWgCYMM6AIqCp174UL79M2CqwTkVQBBjDA6NNBLzap0+f1sXFxY9s2rSJrScLrHZIsDciAgTBWtjk3s5YvzPAMckhvuY2n22zz7uJsUw1lr4BZ3uMZWtVdw7WsE0a+/ilAmyHm84QAlGXI2DAgAH7IU7ezc/Pn2+1L4L9EREgCNbC/mbGArD/mdHzP/s5Zl2Ir9nJZ7ujsdxnLHcYy6Y+x7U0lr6FcXOlxYzZd55ST//MoW8dffb7boeTxkVFRb0TExOXRvCawcK4Cn/fE0GoNSICBMEanF7rLOQpArrCXvDan6Z0l0Co6a+OFNpcmv3xZk1/i7G8WOkYBZfh70XGfrN2vMtYsmXgHKWH+5Gr6umf6Ud/r30UJMdVPTQssMvh/SVLlnBo5Cmwr2AfROjaNeJ0OikCvrTaDyE2EBEgCJHFHPd/n9JN4XOUDrpjTnzGA1AIvAM7CXaL0oXwOFV1DH99YOAdm5LZxz9QHRmKZ+YAYOT+35Uevsd+eQYPMjr/eMNnMxiNLQKskbJQmqd04GAbVf8ofgYujlI6/oCz5HF0AIcotqjuTSHkP7De5eUMQ/DEP7CbJmpEgNvtPhY2z2o/hNhARIAgRJaXlR47z1p+ntIigIXqGUoP1bvZMMIAOt/++1DAYYYsVK/02veYsZ9wWFyO0gKEte+7jf0skDlOvdDrfZlKT4TE1oBxxr5PYafXwz+KjuFKtzxkGvvYVcFMid0CvSmE+MYpOP0eZR3SHSCEDBEBghBZVigdfc9Uvyle+8cpPQkQa9+sfa5VOrveaq9jmCyH4+Z3qaOZDXtb6SGF1cGJg5hhkC0NTMxDIcAx5++rqpMKcXggWwqYRMjMGPiW0gWxN+8Zr7PQphBgM/VC4368C9OPDd8/83n//4z933jtYxWcmQrZosAcArxvCgO2jvRROptiOPl7q1atfrdnzx53RUUFWztywny9oMnLy3MWFhZ2atiwYdQFLAr2RESAIEQeFmrP+Nm/RlUdvuf7vjw/+58I8rpvqqMLe98C2Zd1KrigRA6je9BnX57P9krDfFml/BfqbAV5yTATJkkKezN4ZmYmh2RuffHFF8/bv3//jhrfEEEoALDY9cQTTwSTGVIQakREgCDEN4y4v14FN99AXJCQkJDncrnujzYBQBgPoKQrQAghIgIEIb7hXAD+xurHJZmZmeegoG3fsWPHuVb7EoBjHQ6HiAAhZIgIEIT4hU3LzN1/otWORAtOp/MfEAEP5OXlhXI0Rsjg8ED4JyJACBkiAgQhfmErAIcnRl2ztxWMGjXqXBSw7aK4FcDTHeBwOKIxgZFgU0QECEJ8whEInBnwBKsdiRZQwOZFcysAgQDoIt0BQigRESAI8QnH/hfAdlrtSDSQlZV1HhZR3QpA2BJQUVEhIkAIGSICBCH+SFdHMgAKmqhvBbjmmmsaYNF27969NeWDEISgEREgCPEHYwGYmMg36VBcwlYAh8PRtkOHDlHdCpCamppeVlZW+PLLL1da7YsQO4gIEIT4ogtsmJJWgF+BAIjqEQEG9zz11FPZSUlJzMrIeR3ut9ohITYQESAI8QVjAaYqaQXwkJOTk+FyudpEeSxAe9io8vLyTsakRlmwSUqngBaEeiEiQBDiB2ab4zS/0gpgAAEQ9bEAYL/PdkOl558QESDUGxEBghA/3KN0K8Dumg6MB7Kzs3+DRbS3ApD9TqdzaWJi4g3l5eV7IVreV3qKZ0GoNyICBCE+6Aq7QumpgQWNHVoBPGRmZj67bdu2PgsXLuTMijJEUAgZIgIEIT5gK8BkWLHVjkQDKFTPx6K1DVoBTH7bvn17zjz5k9WOCLGFiABBiH26wYbCulvtSLRgzBTorxWgOYyJgzjlclnkPasKfHQWFhZeUVlZeUaYLsEYgwth78AOheka0QbLvothH8L2WOxLpHDAhsA+V16pwkMlAjgVaY7StQzWOL4N0XmjmcGwO2AM1+VwnY+tdScinK70PPHJsImwxZZ6Exk4uc7DsFZKZ9ibY607deJe2BOwkiCP5/TCjyg9nPBl2GNh8coi2Argdrtb+WkFYNAkv9ecWIk1bsYMFEbaP182b9480Ol0bpk5c+bGMJz+VNiLSv+vf4H9HrYyDNeJJhhU+a7SongL7DbYfEs9Cj9NYMthPWHbYf8xLCQioC1sgtI/HJWYmNgJP7JhIThv1LJr167kefPmTUVNgtHWnNmr880333wZ7t1ttW/hZMaMGTMrKip6cx333HXw4ME/p6enx/Q0tLjnp3HPFD8cT37iGWecsbZfv35FVvsVLMuXL++yevXqoYMGDTqve/fuXYJ5Dwqb/PLyctYM+X/uN2DAgAZ9+/b9GQUnPgKHE997B/Y7fLexdBrLX7d5gNI1EKdxfI3bPCfXA2x7zulvm+dQnn9T9duw32J7NWrXU7Kzsz2v09f58+dfgt92mvExHN+lS5f3LrroomgoEPn9OwRfZ4f6xAsXLszYtm1burF5bLt27eZeccUV74X6OtHEW2+9dcaGDRvMOTPSU1NTJw8bNmyopU6FmY8++qg3ngP9jN8B5w2h2AuZCBiktLLykJyc3K+0tPSDpKSkmC0Qy8rKGiQkJLTHg8Oz3aBBgxMPHDiwrFmzZhUWuxY2Dh48mID/aRoKRM82HvLpsHfxGZRa7FrYYGHWsGHDDvv36xFa+AG1wT0vwX7fIVtRC/5vrSFayrt27brI/L7WRJMmTdJKSko863hP0927d4/E8htdlrpdXGLb7buNz8bl1iXzUds4DY0KwR3MtosfPM4RYNvlZ9tzTZ6D56puG8cfT90Bm+/9On3Fd/skbJsiwI3t1UYkvqXA18uweBS+bAv1uXGPnZVOI+0B4m9DNNxzOOF3WulhsiwQ+Rlsi/V7BgdgfWFJxvavlbdQiIBXlW46Y9OKG4XhO7Nmzbo4BOeNZvjl+RrmqRUfPnz4oxdeeOEca12KCO8r3V/KH863ixcv7q/0gzSWWQS7ROla5MYVK1b0hNml9YMPuhWwbqtWrarNmHJ2e/xB6QdG4dq1awfBtoTDwUiD2vQyPPBvKygoeNHPy58pnU6ZQmDt5s2bR+A4S8fijxo16lQW/vn5+ePDdIm3le4OoBjYAsF3A+75lzBdK1p4RenYD7YGbN+7d2827nmVtS6FHXZjspuvn9JDhB82XwiFCGAgCfvOmIlsg9J9j7EOaxWMLv6r0vf/qLXuRAw2md2u9A+ID6VYFwDkWliu0g+MB5WXgrYBjAVgf35tC7LRsE9gZysdGxATAiArK+sCLFKrGRFAYX8mLEUFHz8RViAAOCQwnP3V/N+yApOiouSeIwBb8i5S8XXPnG/iSlgL5RMIGarAQM5qNTpE57ILjK683WonIgwLk/usdiLC2FXkUbQweHVMHd7LPp8ZhsUSweYFKImEM0HyW4fDcW0ErlMSgWtEGyVWO2ABVUZCyBBBQYhNOMnMJNg+i/2ICtgKgMK0ZbTPFOhNTk5Ob5fLlTB9+vSvrPZFiF1EBAhC7NFD6XHfOVY7Ei3YZKZAk0awl+bMmXNK48aN2XSdoHRzriCEHBEBghB7sBWAw3+kFUB5gusuhABIsVF2QAYnXnbgwAEHjKLl37Bx1rokxCoiAgQhtmAyEAatZlntSLQAAWCbOQIMWipj+JrSo1LaW+iLEOOICBCE2IKtAMx6Z5tcBuEkOzvbEwVuo1YAMic5OXlwaWkph2gyS+BMqx0SYhcRAYIQO/SCZcBGWuxHNGG3VgAGMVZu2bJl/ZIlS6Ypner1C6t9EmIXEQGCEDtw+Cb7jw9Y7Ug0gMKUQyRb2KkVAGIlsbCw8OFOnTqNwuZSq/0RYh8RAYIQG/SBnQsbYbUjUcBNsNELFy48oUuXLhPz8/Nt0wqwZcsWTmbwI3wWASBEBBEBghAbsBWASY3ivRWAKX+ZErX99u3bFYxZLh9SNhhiN2bMmKYVFRWchXWw1b4I8YOIAEGwP5wYZCBsuNWORAH8HNp6bTPSns+5qBcBlZWVdzgcjjenT5/+jdW+CPGDiABBsD9sBfiXste8BmFh+PDhuxYtWuQqLi7m0DrOcLnaWEY1Y8aMaV9RUTHG5XKdZLUvQnwhIkAQ7A1nBeOkNzdY7YjVZGVlDUVNuuC00067/q233uqidOH/pMVuBQUEAEcxzJwxY8Zmq30R4gsRAYJgb/JgE5Se6ChuGTVq1AgUopzl8WIIgC+t9qc2ZGdnn4jFVeXl5SdY7YsQf4gIEAT7wqbj02G/t9oRK0EheicEwKjKysrzZs6c+aPV/tSBhx0OxyOzZ88usdoRIf4QESAI9iVPxXcrgCMrK4sjIi6AnQ0BsNVqh2pLTk7OQJfL1b9BgwbXWe2LEJ+ICBAEe3Iy7FTY76x2xAqMpDpPYfXYsrKy8+xai4YAmOB2u+954oknoj54UYhNRAQIgj3Jg42HHbbYj4iTnZ3deMuWLS87HI7KJk2aXJSfn2/LlpCsrKyrsGhUUFDwvNW+CPGLiABBsB9sAWBLwDVWOxJBUmA9R48evaaysnKx0+lcu3v37kwIgKgf/+8PMz2w2+3+IzbdVvsjxC8iAgTBfuQpnRUvXpqQx8H+jJp/q1deecU1ZMiQ/Dlz5vzJaqfqw9atWzOx2FhQUPCO1b4I8Y2IAEGwF6cpnRvgaqsdiSAsMDui1qx27tzpggB4z2qH6oORHvjvEDUXW+2LIIRVBEycOLFRkyZN2rhcLmc4ryMI8cL9998/oXnz5vnjxo1LC8f5nU6nCwXU7tzc3P3hOH9tGT16dNcXX3wxrbi42LMNIcD+/8bWelU/8PnejsU706dP/8pqXwQhZCJg8uTJ7RMTEy+Dur0MP9TjlZ7II4Wv4cESqssIQlyTl5fHRQbs/nBdA79jhQJqH1Y55O4n2OsQ8gtRIG8I1zV9GTt2bPPS0tK7Kysrb+7cufPbJSUl/fFcScZLX8BeipQfoSYzM7MdFowDOMVqXwSB1EsEzJ07N2H37t03oZDPxg+UzZQON9vslNoMMfADllvx8CjCum2m8hSEaOXLL7+8pGXLlhuOPfbY78J4GQestdIinsYx+EPwG38MwuBbrM9u0KDB5BEjRoRlVMI111yTgHvMhADIw+ZreH70/uqrr7ZjvYlxiK1nScTneB+eh7PxWf5itS+CQOosAqZNm3Z5cXHxeHyhe6LcZ9PhAqwvLi8vX5Kbm7sthD4KgqDUGTD2IbNQLovURWfOnNkMv+nB+G1zSt5LYY+WlZXdikLs3q1bt87Jy8sLmcAfNWrUhXiWTMS1irA5xKe53NaFP8nKymIL6TAIAUkPLEQNtRYBM2bMSK2oqHgeP1TOec3aAMcqj8cPeE/IvRMEweQfsIdUBAUAGTlyJLsFXqHNmjWrIQTALVj/G2x2Wlra6KlTp149evToLfW5hpE7/1EIgOPwXPkzCv9FIXA9mmDT/4NvvfXWiQMGDJgyd+7cYqsdEgSTWokA1P57VlZWLsQPtRt+sHMTEhJuh7qVWa8EIbycBWPtcZaVThhdABNQEZiB5wBFyR9Rq/2soKDgt3gOrKzt+SAwUvH+PKz+Ds+Tf+K5chUEQHmI3baarrDFsHYbN25Uv/zyyxlWOyQI3gQtAqD4M/AjXYjVJljegZr/o2H0SxCEI5itAFFRQGZmZu7GYiwK7I+xnOlyud7H+o14Jrxczdvawh6DOZs1a3bf9ddfP0TpFoUXISh6zJw5c3cEXLcCdqG0MzcgdropHXchCYKEqCAoEQDh3x0/1HlYTcCX+HL82JeE2S9BEDQDYd1hsy32owp4Djyfn5+/Ds+E17A5Z9q0aZtzcnI+9nMoo/rfUHrWQwbHXXno0KH3mjZtei4qF2sj6XOkad269ZsHDx4shfEzYMG/QYkAEKKIGkUAFH4LdgFgtaXD4bhGBIAgRBS2AjyooqQVwJfs7OzPUPhfiWfDUtiCyZMnn5abm7uJr+Xl5Tk3b97ce8+ePYMXLlx4YmmpTnC4b9++sjlz5ozFqh2n/Q0aY6TDo2vWrPl42bJlDHbcBfuz1X4Jgjc1igD8sJ+G0u+B5X34wc/zdwxk7XlYnAvbAXvdodTGEPsZdbh1/nZGajN5yVu45zUWuxR2cM+9sLgIlgR7G/f8P4tdCjtu3afLputU2Pu45+URvPw5sGNhT0fwmrznjlhcovRyBe757eqOZ+0fQiCbzwrYO6goPIfdZxUWFg5ArX9T48aNP1G6AOzE410uF+OIdoT5NiwnNTV1Bu41YdCgQRdCBFRY7Y8g+KNaEYAf9iAIgCvY3AcB8ADsqNfdujBgywALxBZK11Yewv678OCYHi6nrQb396zSD0neM4dI7cO+xx06p3tM4tajQPgFaKZ0n+ZfsW++Q6d0jUlwf4yEz4M1hSXA9mLfR1gOxX1HYuIasxUgYgWIW09NPFXprHwNYCXYtw7L3+CeD3ofi8K/Y2Vl5UAU/AO//PLLgZ07dy5r06bN8R06dDgVAmASDvk4Pz/fHDWUDpsIY+Yw1oajdTRRZ6VTMnOa4jr7iGclpwg+Hp/NhXl5edEuANrAboBRvBVZ7EukaA4bAXtVxUGl1aAR7CbY+8qr0hpQBOAL7MAPeILSD6Db8GX214/FrGVnqiOJPPjQYI3pYRy8DA+NcCY1sQTc12gsLlNaABA+1FrCbjHu+V3LnAsTbh3cRAHQ0ms314fhteWOKOyvri+4r/5Y3KeOvucUpVu87oI9EGYX2LrGAmlOmK/zK259vcmq6v+5H5TuFNTw/4OarafQx76BWE/G+gqsr0CN/7mmTZvyYbq2ffv2/Vq2bLn0lltu8Z7giK9F+3wHLBQeUbpQvBU2CLa+tifJysqiyLkkISHhnKlTpx6s8Q3WkqF04d8BdifsRlisT2rE4EzOP8Hv+12GzbTUo/CTAuMInuNgHKL6pDIqrQFFwPTp06/FD/xkiIEpUPzrAhw2XB0RAN7wwfE0Hior6uF0tELF3MLPft7zNLceDhRrXKmOLhhM+Dk8YhSYsUaG0oLWF7YK3IF7bhXOi/9Vqd640Lp7lIrkKJz+yv//OflwcvJwPAvOxTPhG2wzc+BiiIBt2HbS8Frqd9991+aEE05YBDFww+HDh6egMGQrIVtQnOZxXOdcIiCBS3OfYb8e6/NagnmM92u4ZoL3+733BXNu3/fPmzdv4K5du8z/azr4cMiQId/U5gPEOVmYMpDzU1znBQin2rw94ixcuPCkrVu3tjU2od/aP3/FFVd8aalTYebNN9/svWHDho7GZttWrVr9a9iwYTE9LfeKFSu6ffvtt92NTX7HL1c1iQD8SPmjdyUlJT1UzbkbVfPaqYbFE/yQb7XaiQjDB0i83TO7RMJ6z+OPrA4J53WCBSWmq0FFRVFpQkIrFJxn49lwFmq6Lj4j8LK5rFy/fr2jT58+lQ0bNrwax7WAVXofQ+MkRaCSS/M1w9jFwtdcPq9VmscY7ys3zlPp/X7C6/m+n0vTD/P63M993n6Vl5dTAJ1j3LKb2U+x9BsH5Q+cYwAWt+C8DHqsVwKlSFFWVsYWvqvM7dLSUo7umGqdR+GnsrLyCiyoztityc+AQm+SlT6FG6b1x/fyLiwbGrt+HZLrVwQ888wzTQ4dOnQ+VleOHDmysJpz8zV/NQdmNWNXwnN19DmaYV/hmX7284HyPOyfkXUnIrDpmw+KBD+vLYWNiaw7EWGk0gV9kp/XVsOGRdadiMC0xOzia+r7QsOysm1PPvVUUIlupk+fPjc5Ofma008//fbMzEw79beyhYOxTMdwvbCwkOmRg2rOz87OPhuLMdAfF82YMeOLcDoZYj5XuhuXzcTri4uLM3HPsR60+b7SlZfesE379u3LwT3H9EgV8Casi9KJxxj3ca/5gl8RAAFwIRZUDK/WcGL2jzPFp3fzOFU25w74p0NHzscUbqWysPhQVW0q9gz/ccRgYI1bF4bso27j8xLveZRDB47FFG4dlHe90n2lDq+XSmAjseN7K/wKJ249ZI81Q7ZoeT8bSpQOkgzuPG73PA4nRoF4pdIJguwCZ0zks4/dA0HPiQAB0AeLV3DPv7eZACA7lY5xqtU92xxmvmR8SjzdM2P6WLGpcs+BugM49E0Z+QECggfhMiOaOF/paGI+LL+G3RCLAoDgvla79ciAZ5TuW+E9b4BdF4sCgOC+Ct06IO4FpWtJhPd6YywKAIL72u/WLT5s3TlR71LMo5+JlU8sdS5M4L4q3Lo5nEMSzVo/f8e347X5wZ4Hz43FiYmJbLJngWonEWASdMGQk5PTBWJnCQTAH1GbtHNQcLwUht7IPasAIgA/3nT2lxUVFdX4gHfoZoZ0t+4WOOCI8AQnVmAUAie49TCT8lgVPN4YNd+T3FrsJTh0gRjT4B6Z9OYct85418iha8QxjUPXDC81hv82d+jWnlqRm5u7HwUicwEcU+PBNgYCoC0EwFt4Vj6E+33Fan8EoS74FQH4UjOpx/baTBPq0MMO4grc816rfYg0vmPF4wHcM4e6ldZ4YAzh0Dk/ai0AvNgKOz5E7kQdN998czMIgNexOgcCYJrV/ghCXQnUHUARYKeAHiH8sKWHw2q+tdoRwRZQBJw5d+7cBtdee21MtQ6OHTs2ubS09L9Y/Sg/Pz/c+SIEIawEEgGtYTE9VjQGYbTrcW3btu2Xmpp6mtPpTHW73eymOIwl7VBFRcX+gwcPbtq6dSsToLCm5/Ixd6B9KSkp1/fs2fPWoqKif/7444/j/TkgCF6wFcGxZ88eisftVjsTKjgfQmFhIUc9bYcACDpYUhCileoyBspMV9HNsb169XqoTZs2XZs1a9aia9euTfr379+iR48ezY877jjVokULjvJQhw8f9pi5vmvXLoWCvLKsrKwSuGBur+Wv5nK53BAN3K+Mdfdpp52W/OWXX/7ps88+u5njqr3x3TaB6NhbXFz8+oYNGxhk+rPVH5oQMTzPD3zPHDUdaBM4PLYSAoBj6JknghH18owUbE9QUwkLUUers84665358+d3bdeuXcCDmjZtqkpKStSTTz6pPv/8c8Z6MAkUlwko3BNQsKvy8nJGc6uaNB9FxWWXXabuvfdetji0rfZgL1AIKIiGwTNmzPjbypUrH/r++++fCPouBcF6GBQ6F9YzJSUlEYJ2T8OGDQfOmTMnKmd1FITaIiLAhvTq1avg2WefrSIAZs6cqWbNmqX69OlzlI0fP1599dVX6umnn/aIgW+++cYjDqqjZcuWqkePHiojI0MNGTJE9e3bV3344YdqzJgxavny5cws5jknHojVnqdBgwZq4MCBtHaXXnrpOIgADieNqyA7wdaMU8Y4ev5mFgCI5wMW+yQIIUNEgA1B4d/p2GOPrbJ/9uzZzBHtMW9OOOEEtXr1ajVx4sRf97H2z4KcLQFsEeC2uWzUqJFq3bq157jCwkI1fPhwtWzZMk+t3puPP/5Y/eY3vwna76ysrI4QIdcUFRU9W5v7FQSrcDqdJzILsbmN30iKhe4IQsgREWBDGjduXCWtK/v7P/roI7/Hd+nSRSUkHJ3xl9s4T43XSk5OVu++6z8HytKlS2slAi699NLkDh06ZIkIEOxAdnZ26507d574+uuvHwKcJ4UBjjE7RboQn4gIsB9JzZs3r1J6r1mzhgF8ft/AZv260qpVK088wJ49VadWDyQ6ApGUlKSOP/74zqtWraKI2V9npwQhzOTk5JyE39N8fP+fhRC+GCKAs4cyuPUXq30ThFAiIsB+pKWnpyf77vzhhx8CvqFr1671umDnzp09cQS+bN9e+5FfAwcOTJk7d25PrH5aL6cEIUyMGjXqBgiAf7vd7pyCgoIFxu4nLXVKEMKEiAD70aFbt27NfHf6q6mbsCZfH1iD98fu3bv97q+OU045JaVt27anFhUViQgQooq8vLzEwsLCf6Hwv8ThcPwmPz//O6t9EoRwIyLAZjRq1KhjWlpaE9/9+/cHbl2vrwjwjScwqYsI6NWrl0pNTR0IETClXk4JQghh/z8EAIcCMi326dOnTw+sqgUhhhARYDOMKH4O6j8qCQsj+wMRqCZfXzi6gHEIzD0QLCkpKRxW2DEsDglCHYAAOBmLeW63+9mOHTveV5s5UwTB7ogIsBllZWW7AOfDbuS9PzEx8L/Sd2hfbTlwwP+waA4jrI0AIBxrDcGyrV4OCUKIYP8/Cn/f/n9BiBtEBNiP3du3b68iAji2PxCssdeHQCLgmGNqP1PsF1984d6xY8fb9XJIEOrOjbCRsIaDBw/+CYX/KdL/L8QzIgLsx+6ioqIqVfvqRgBs3bq1zhdjcz8Kbb+vderUqdbn++CDD3ZCxCyvs0OCUHc6wP4B82TaWr58ee/09PSO0v8vxDMiAuzHLhTKVQIAOGlQIH7+ue7z9vz000/q4MGDfl/r3r17rc/36aef7sUi8HhGQQgTZ5111vn4/nUy42cOHDiwJz8/v4HFbgmCpYgIsB+HiouLq4gAtgSwZr558+aj9nPSoPqIAM4PEIgLL7ywVudiPEBhYSEFgMy+JkSEG2+8sUmjRo2Y6CfX5XI1WrVq1XaIAAamMrCWX27/zVyCECeICLAh69ev/37jxo3p4Kj9t99+O19T3bp184gCc8l4gU8++US99tprat26dWrDhg2eOQM49I8BhUwNTONkP1ympaWpfv36qVNPPVWde+656rHHHlPz5s3zTBxkZiXkOTm5UG2YMWNGyY8//vhQqD4HQQjEqFGj2DSW63a7GQOwAss78P17S+lZAe+FbYHNsdJHQYgGRATYkO+///7GK6+8ctmECRO6Dhw4MMmcA2DcuHFVjuXEP5xKmGmFOb8AgwQPHTrkySvAZU1QaOBa6uGHH/YIigULFnjSBXM+gppmEPTlpZde2oRrrqj5SEGoPXl5ec6tW7deqnThfzLsKQjdU6ZNm7bB6zAG1d5tjYeCEH2ICLAnO1atWtXvD6Bly5YXtGjRoiOWqT169Gh29tlnt+nbt2+Dpk2beroCOG/A1KlTPeuEhT/FAGv9bAUwZxGkUSBwJABjACgQTOPxn332merZs6caPXq0x2oDZyJ86KGHikpKSiaE48MQ4o5eMGbAWg3bk5ub2wrf45H4nvGLyeb9yWVlZVfOnj37sKVeCoINCCQC9qHQaB5RT4TaUopaTz7N3LFkyZJjJk2a1A819nNRwKc4Af6PHMifYCzJHpfLdQDbHFPYiEvUmLjeEOtJeB+PTUhKSkp0ajxLiIe0duCBBx5Ym5ycXIwaluf0+hIOzxLvNfc5zf27du3ah/euLSoqml1cXLzMig9KiDx8fuB7pZo1a7YvxKceB7sH1gLf0R+uu+66VRAAl2D7v7jetQUFBZ+F+HqCENMEEgGMLkuLpCNCSNhUWVm5ad26da+F+sTp6enL77///m/XrFlzOTbrl31IiAf4/Ng7fPhw/0km6gjE5QiI2FZcR+Hf89NPP/3xggsu6D558uRdobyOIMQLfkUAVPxmqOpzI+2MEL1s3LjxbKt9EOwDnh8UAXVPUOEFI/wbN248FOe8ft68eb127fq1vK9Yu3btPJgIAEGoI35FAH5sbAlIhrpun5ubG3yKV7dqj7/7lEOFVP1HNW7VGn/Lcc/xk3DErVLwNwH3HD8PX7dqir9Ncc/xk/LYrRj52Qr3vKU2b5s7d25CcXExM0kFO1Mkr3MG7H8wz+9o7NixyaWlpRdj9TrYENT+VzidzhewZFzJdOM9K5VE+AtCvQgkAlY4HI4RiYmJg7H5dI1ncauh+MtZ4ThTTQK212N5LR4eG0PpbFThVufg72wYYycc2GbhcD3WvrHUr3DiVn3w9wVYW2Ob/b034p4/stKtsOJWTMXIe+7m2XJ7osvH4J4XWetYGHF7/r/PwU6CVXruWqm/4p5nB/N2CAC2InKmy2AyQ54Ae1HpYL9tJ5988sRTTz21LwTAlXgOfYXn0ItJSUm5Ps39vWpzO4IgBMavCEhISHjNpQeEs3CvXgS41SDjmBSvvamKDwC3OjEmWwXcqjf+/hfW0msv+ynfw2v9cM+F1jgWRtyelKvvwtp47eX6a3htIO55jTWOhRG3aoa/HyqdbtZ71sY5eO0K7PnAGsfCiNvzTGAAJ0WP9xzSj+G1StxzjTVvFNxXMSgQj5BfJ+TJzs5ujJp8S+xLxWupOKYll4sXL/5DYWFhf+OwYzZt2nQ/RMB9FRUV9zz11FNW/o4oZIbD/qP0KIR4oC9sLKxABd+KY3f4Pb8T9gosXuY04fPsb0o/217xKwKysrK2T5s27VP8UAdPnDix0W233VbdgPIn1dECgPCBydrEA3hwPBMCp6ONqepoAWBC8cOHZSwmxOHY6lZ+9vNzKMA9/zHC/kSCPygtdBw++zk8bQbu+ZrIuxR2LoAxo16Cz362eD2cm5u75PDhwy1RO0+trKxsiYL9qEIdx6SWl5dfi/XSr7/++lkU/vx+cD9FwW4sduPYYnPZoEEDthj8OjX2jh07lubn5/8nUjcbgFthDyrl6QK6SunKUKznt2B+BQo8/r+uVfozmG2lQxHgRBiFPMsqZpWcqHQiqViG3ddsuWWmuSzYhQHzBOAHOheLMxo3bsyxtxOrOWmHAPuZk/tPhsULHIY3zLB4YqDS/bnxBCdOiJt7PuPlM1SXr7t0LKsoK0pISKhEgV6JZ0QlCnu2GHJZiaWrVatWHGbaqKioiHNEMIsVJ55gt5GZKprPnDaGqYsuusixf//+QzhfA54Pz5u+OP/3FtziryxYsKAzxIg5LWfqMccc88bFF19cq7gIu7F48eKOW7ZsaWpsNm/fvv3koUOH/tVSp8LMO++80/6nn35qYWw2Tk1N/fOwYcNiUdj/ysqVK1tDnJuVOWbPPCOgCDhw4MC0Jk2a3IYf5t2zZs16asSIESWBDlW6luAPjtl9r84eRy8jlNkvXpUfYfMi6Iums7pJvaIWqNPDFqB4tdIFnz8YD1Fz7Ij9OB92WoDXWLBNiaAvkYJxAGwNcHrv/Pb8b9X609eXDB0/9Hyny+l37ofk5OTE9PT0BRAESfv27WOffnEwF8RzRu3du7chxYXT6SxnV4KV4NnH2uBvjU3X4cOHn4RPL1npU7g5ePAga4U5yvi/Y3se7rm6yp/tKSsruxDl24O4T88kUrjnD7Ae08KnsrKyJ+55Cu7TFD+bA4oAdgFMmzbt73jDU/iw7lK638Qf7D+6TemmM2/Y9DdcOZSlqj4suNVa/J2kqoqfElg27nlphD1ia8xINUCNUeGanMetmHed/Wa+3SCs8f0Z9/xcWK5rJTr2g82FqT6vUPj+C/f8QOSdCjN6hA+DW1t7797fan8pbP7MaTMDtn5Mnz49F4tOeGbcN2HChEj/BkIJC0TmwmBz8codO3bcV1BQEOu5Me5QupWGovdriLJbcc+xPuKJE0jx2cn4j58h9m7BPcdePNfRrFK6Msd8LxzCe2e1aYO3bdv2dFpaGgv42yEI3s/JyVni57D7Yf1hHEfOQpEz3PEh+ZeYFADEoWbhYXmm0rVjBo+x4N2vGA8QeQFAMpQO8ghfFcrhCXqcpHRfIftxWWNgbfjlmBQAxKG+xT2zZjBe6aZtjn7hg5GBc7EY96E8QyDdEJRKPQXzZJJUWtwy8HNsoLdBAJysKIyU2l5eXm73GiQrMNdb7USEYbfNKKudiDB8XsZTd7XJ/YZ5qFYE5OXluaCMrnW5XCuh7l/AD33AqFGjji7YHYp9gVfg46SC5LA5/oDewf7Nfk4ZOzhQ43erJ7D2G1ip0ve83iJvMmDvh/0qDnxx3Op5rHFECJvQlnoKyljG4Ql6fB1rnDeZrSDLsC+2U9M61ELcc0+l75lBgiux78NAh+MZ0Q7PiFexmojldbm5ufsj5KkgCPWkxgmEsrKy1uTn51/ndruZinYRhMAgCIFfqhyoH4yx/XD0RecEiIa8ABQij0fkSg5PzMOPEblWtKAF7Syr3YgoDlWkVM0tPFOmTGlZWVn5KioJx2Bz1OjRo98Pu2+CIISMoGYRzM7Ofh2F/+1Kj5n9bOrUqb/Fjz3Wh8zYBdbUUlT8jGUWooSCgoIeqPkvxGp3iICJeE7k1/gmQRCiiqCnEkbtf1J+fv4ut9ud73Q634MoGLd169bp7DIIp4NCjWQoHbxmbUi1EFfgWXA1BADjBhgf8icIgEkWuyQIQh0IWgQQ/NDnTJ06dS1EADOBTUlLS8vBw+BO7H8zTP4JNZOhIhEPIAjKEwA4ABWBf8EY/1OM5cU5OTnxkmlNEGKOWokAMnr06E9nzJjRB7WAu/EAyIW9MW3aNEZLv4x9i8eMGfNTGPwUAsN4gElWOyHELhD+bRMSEi7D6tX4vV/icDhcWM7GvnuzsrJiOwBYEGKcWosAkpmZyREAt+Ph8ITT6XwAD4XfYfscPBQeR02BQ4k+V3oMIq2ID43QuSyYFBYWpr7xxhttbrrppr74P/Sx2h/B/kDIM30vcwRwKuA0/HZ7YHkaCn0OCWXh/xr23YXafzQExAqCUE/qJAJMRo8evQGLGx9//PHcBg0acNrPy/GA4PJG7+OszgAWq6SlpakRI5i8UD0vn3F8snHjRpWSkqJatGhR88FBgN+v7679+G79FyJzYWJi4uKbb755R0guJAhCVFAvEWByyy23MGvcS4YpiILmeGCkJSQkMBsTaxMNQnEd4WgWLVo0onnz5r+cd95571rti2ANU6dO/cvZZ5/92uWXXx6S0SEul6sCBf42LAthW8eMGRNU6l9BEOxJSESAL4YooK0Nx/mFX2GO89HPPffcd1Y7IlgCxfXjEIP/gEmCHkEQak1YRIAQEZichfM1iACIX05RWmiLABAEoU6ICLAvGUrnBxDiF058ssxqJwRBsC8iAuxLhpL8APEOx+rPtNoJQRDsi4gA+8L8AI9a7YRgGRyydxbsJov9EATBxogIsCedlZ7ado3VjgiW0Ru2DbbTakcEQbAvIgLsSYaSeIB4h10BEg8gCEK9EBFgTzKUxAPEOwwKXGS1E4Ig2BsRAfaE8QATrHZCsBS2BNxptROCINgbEQH2Ix3WCPa91Y4IltENVg7baLUjgiDYGxEB9iNDSVdAvCPxAIIghAQRAfYjQ4kIiHdEBAiCEBJEBNgPxgOMt9oJwVIYFPhvq50QBMH+iAiwF11gyUomZopn2sNSlOSIEAQhBIgIsBcZSroC4h12BSyHua12RBAE+yMiwF5kKBEB8Y7EAwiCEDJEBNgLxgP802onBEthPMAcq50QBCE2EBFgH46FJcF+sNoRwTJaKP09+J/VjgiCEBuICLAPGUq6AqINTuJ0AOaCJUTgegNhn8AqAryeB7tP6dEjf6vjNU6BfVHH9wqCYDNEBNiHDCUiIN7hrIGPhfH8l8LugZ0ZxmsIghBFiAiwD4wHeNBqJwRL+TKM52Yq4pdh34XxGoIgRBkiAuxBV6Wbm9dZ7YgQszD/QCOrnRAEIbKICLAHGUq6AuwEA/geVXo4Hyd8YjP+h0qP7DATPfWFLVR6IqhpSvfld1c6CdBdsHd8ztkSdj/sMuP8q5Ruuv8oCH8awv4Cux7WQenaPuMGXjVePwv2irHeB7YBNhL2bhDnFgTBxogIsAcZSkSAnWChfp2xvl/pTI+0DFh/WLHSmR8pEFgDvwjmMI4/DfYGbLA6Ugg3gX2mdJM94QyC7B5ivoBhsAXV+OIwznee13sHGO+5Q2mxQh/XGb4cVFpg7KnF/QqCYFNEBNiDDNgDVjshBM3NStf4n4TthB0Dext2gtL/S+9Cm2KAtXD+fzsrPSfA8UoX0KYIYCsBBcAKWA5sO+xe2FhYPmwJrDSAL6zRUwCsM/xaBRsNmwB7SOk4gK+VHk3ATITrYVfW8b4FQbAZIgKiHz78nUriAezEIaWH65lsUrqwZfN9d59jK2F/VLpgZ2HM5vrpsOO8jrnQWD4M+9ZYZ6E9AtYa1lMFzh1gvneq0oU8+ZfS4oCihK0CG4O6K0EQYg4RAdFPhpKuALvB/AFjlG76b2rsO9FYOn2OZUzAdq/tD4xlktc+sxvgT0rX/k3KjWV1CYTM916rdBeDSROv9wqCEKeICIh+MpSIADvB3xSb7fsHebyrhm2OCmlsrJ/t5/3sBqhuMqHmxvJkP8eV+rmeIAhxhF1FAGsvDKraYFhtSFO6GZQR29+HyJ9WSkdV71a6STeUvKaONOOGi3B8JvEKE+70VzpoL1MdSfPMPv976nA+dhdsULpGz4yBtc3m96PSXQs3wV6ow/UFQYhh7CoC+HDlMCo+WP9ey/dernSf6zOwPwRxPIdmcdKW/1ZzDIdYcbgXg78uqqU/NfFSiM/nj9p+JoLGrFkzAj9VaRHYwdjH5nnveR781eKDhcKMIoAxAN4i4GrYYthhn+Nbea0zQPFi2HDYXKVFBRkC+1zpwEVBEOIUu4qArbCvjGU4Yb8pI69ZG69OBAjxCQMAWfBTADCn/21KdwVQHDACn60rHJPfQ2khWVf+oXR/fq6xfE/p5v1TlW5xOF/p+QQ2G8dnKT3cb6jSow0o7FjoM0CRoqGr8R62EnBIYkk9fBMEwcbYVQQ8aVi4+SvsDKVFgCD4g0P1mM6ZUf8M/lukdGIe7htsGAvoVUoX3HWBOQJYk59gXMccYcCo/r+pIxMKzTWOY6vD5cY+CgO2GPD3wlEEmcZ+5irg97ukjj4JghADRKsIYA28jdIztO2AtVO6Wd7sr04xrERVfYiZ060y8xqjpzn2mjWzQMOgWGPqYBx/yGt/qvGaeb30as7hC4/vAlutjkRw+8Lobz6U2Q+/3c/rHFvOoDDW3iq99nc03rvFOHcDw/8yWKHS989C4ielH/SBYPOy2zhOqDtTlO5KOUkd+T9yCN7zStey+b9i1wB/axQJq41j+F1mwp/9PufbbOz3beJnf/48pYVEJ+P9NO8ZBfcq3eLQVunWB5OlSses9FP6/74B9o06OrcA/WK3gSQJEoQ4IlpFALOgzVZHkqBcZeznGGlmYvu9qhoTwCho9mmzBsRhWBQQTLLCjGhmYelNitK1tsuMbRaYt8CeNbaZotWsNf0ZNk4dPWzLHxQvM5Xuu2U/MUUFa4VPeB3DFK6TlG6ibWjs+8G41jKv49hfy4c5xccvXvvZFMxkMnygMwjRHCPOpt0Fhq8O455ZGN3t4yMLKxYm5tAwfqZLa7gvoXoo0j732bfFMG+8gy73Kf+jPg4F2E8o9FYG4U+RYd5wFMD/VOChhCVKZxYUBCGOiFYRYHKJsWQhyNoPC6zVAY59HHaFsc5YAWZie7Sacw81lqx5sXbNloanjWsxzzsLyl5KD9GjGAnmAckAQU7DysQ+bL1oYfjFfmEz+xsFgSkuWHNkshcW6swVz9pbXWvmrP0zyxxbTpopLTAolBj49R/jGH4mrxu+VRh+smbYu47XFARBEGxMtIsAwiFXLIRZUO8KcAxr/jcY6+znfETppnTmcM8M8B7CVgP29zNQ6m1jyf5TioClxrUzlJ6k5Qn/p6jCBUrX1nn9d4z3/5/SIoC1b6ZuZc2RLQGM/GezP7O5se+Y/btZQV7HH+yf5r1TBDyndDAYk8SYIoC+UQCwCbq/0ilizcloutTjuoIgCIINiXYRwBEAS4x136ZVb9gVwEKXzaX/NvZVGuuBRACHWpkBf6x9z1G6+yC9Hv6y1v2e1/XnKy0C2hr7GC1OwfKm0oU0+Vnpbg2KgPrUyDnxyySluwEYsU7RQhHgfT9tjCXnpV9vrLMPmK0cOfW4tiAIgmBDol0EBJu4poWxZByAd6BUdUFOvt0K3xlLh++BtcC3Kf9nY2n6Z44h3+ZznDm065h6XJvdGN5BZv7ux/TD108JDhQEQYhDol0EBIuZ+tQ3L3t9CvS64Pt5mtumf2aUv6+fCcayQoUXM7mNb4BjrHwPBEEQhFoQKw9/s8bP6HwGxJnDq1pH2I+uPttmBL7pX6Gx7ORzXBdjuclrnykIOMzMHB3QStVvwpcSH79MfP0WBEEQ4oBYEQEcbsWhhIx+H690gB3X/xFhP1KUztHOoYrMMzDc2G8W/uyCoJ8ZSg/d4xhzjt9+xHjde5gZ072y+2Cc8T4O+WLMQk3DFKvD9OMUpeeY54x1zCs/NOA7BEEQhJglVkQAm7mZsIXj/G9VehpXNrknVPemMDFL6cK9kbHNroAZxjoL4YlKixRmlMtTR/4HbC0Y73Ue5htg0hhmfzMTH1EA8F7r2s3BEQ/MJ8DhhO8rHUzYuLo3CIIgCLFLrIgAwjHyrP3/TukaOYPyWHNmrby6qVZDyfuwT5UWIYQR+Lero2v4bAFgZjeOWuAYfQYzMt8854n3zhzI0QPMXcB7aG0cl2ec+/g6+seYhEFK50PgjHT8vDiMkQlo6jLDnSAIgmBjolUEPG1YIO5WVTPhcXggh7lx4pYUpTMAXmy85p3uN98wX+Ya5g1zDrBwDJT614SZB71r56zpc9hiiZ9jKUjGG8ZofUb0V/o5jphzJFAEmLO9PeZzzCrlv2VgQ4D9jC9gC4PZUmGmSr43gA+CIAhCjBKtIqAusFbLAps1aA4t5JwBdxqvBUqVWhMVqm4R++wCKAniuGDztIdjutdDNR8iCIIgxDKxJAJYu2cgHoUAo+wZgc+a8AElTd2CIAiCUIVYEgHsBmDt9np1JOkOp2BlsOA6q5wSBEEQhGgllkQAI90ZE5CrdHpeBtkFmmtAEARBEOKeWBIBJgyy+67GowRBEAQhzolFESAIgiAIQhCICBAEQRCEOEVEgCAIgiDEKSICBEEQBCFOEREgCIIgCHGKiABBEARBiFP+H5vs+GOvBd4HAAAAAElFTkSuQmCC)

## Difficulty Levels

The **Static Reaction** and **Dynamic Interaction** levels mentioned above are **difficulty levels**. Scenarios can be launched with different difficulty levels, corresponding to how "hard" the scenario is for the autonomous vehicle to navigate. Difficulty levels can be specifed along multiple _axes_, corresponding to different _types_ of difficulty.

The Static Reaction level corresponds to the difficulty of _static_ objects. This includes
- where static obstacles are placed in the scenario (are there none, are they on the road, are they in intersections, etc.)
- where is there occlusion in the scenario (can the autonomous vehicle see everything, are there blind spots, etc.)

The Dynamic Interaction level corresponds to the difficulty of _dynamic_ objects. This includes
- the difficulty levels of vehicles (how many vehicles are there, what paths do they take, how do they behave on the road, etc.)
- the difficulty levels of pedestrians (how many pedestrians are there, do they follow paths that get in the way of the road, etc.)

When you run a scenario, you have to supply levels of difficulty to each of `static reaction` and `dynamic interaction`.

### Creating Scenarios With Different Difficulty Levels

When you create a GeoScenario file, you can supply different difficulty level tags to the different objects. These are the `staticreactionlevels` and `dynamicinteractionlevels` tags mentioned above. By specifying different difficulty levels for the objects, you can create arbitrarily complex scenarios.

Since there are 6 levels of static reaction and dynamic interaction, allowing for 36 possible difficulty configurations in total, it is often reasonable to
- A) assume either the static reaction levels or the dynamic interaction levels are capped at some low number like 2. A common option is to limit the static reaction level to either 0, 1, or 2, where 0 corresponds to "nothing", 1 corresponds to "occlusion", and 2 corresponds to "obstacles on the road and occlusion".
- B) try to create scenarios where the difficulty levels do not conflict. For example, it would be difficult to maintain a scenario where dynamic interaction level 3 and static reaction level 2 includes an intersection with an obstacle in the middle that an oncoming car swerves around, because you would have to ensure that the car's swerving path doesn't interfere with the desired behaviour of your scenario for _other_ static reaction levels. (i.e. would it make sense to run the scenario with static reaction level 1 and dynamic interaction level 3?)

Additionally, maintaining 36 different difficulty configurations at once can become cumbersome in JOSM. To mitigate this complexity, it is possible to split the difficulty levels into separate `osm` files. This functionality is described in the **Multiple OSM File Structure** section.

### Multiple OSM File Structure

A scenario can be split up into multiple OSM files corresponding to the different dynamic interaction levels and static reaction levels. The folder structure **must** be as follows:

```
scenario/
    dynamic_objects/
        scenario0.osm       <-- Dynamic Interaction Level 0
        ...
        scenario5.osm       <-- Dynamic Interaction Level 5
    static_objects/
        scenario0.osm       <-- Static Reaction Level 0
        ...
        scenario5.osm       <-- Static Reaction Level 5
    scenario.json
    scenario.osm            <-- Base OSM file
    scenario_config.bash
    scenario_route.gpx
```

The names of the `osm` files in the `dynamic_objects` and `static_objects` folders _must_ be suffixed by an integer from `0` to `5` (inclusive) indicating the difficulty level the `osm` file corresponds to.

The Base OSM file includes information that is common to the scenario regardless of the difficulty. The `dynamic_objects/scenarioX.osm` file indicates _solely_ the information that is to be spawned at dynamic interaction level X. Similarly, the `static_objects/scenarioY.osm` file indicates _solely_ the information that is to be spawned at a static reaction level Y.

You can create scenarios in this format by running `bash scripts/create_scenario.bash` with the `-s` option (or the `--split` option).
You can launch scenarios in this format by running `bash scripts/launch.bash`. The Scenario Manager is intelligent enough to determine if the simulation should launch with the multiple .osm file structure.

When you run `bash scripts/launch.bash <scenario> X Y`, the GeoScenarioParser will load the contents of
- `scenario/scenario.osm`
- `scenario/dynamic_objects/scenarioX.osm`
- `scenario/static_objects/scenarioY.osm`
as if they were in a single file to run in the simulation.

**Note:** The `dynamicinteractionlevels` and `staticreactionlevels` tags on `vehicle`, `pedestrian`, and `staticobject` nodes are _optional_ in the `osm` files `dynamic_objects/scenarioX.osm` and `static_objects/scenarioY.osm`. If, however, they are included and they _contradict_ the inferred difficulty level from the file name (e.g., a `staticobject` node with `staticreactionlevels == 1` is in `static_objects/scenario4.osm`), it will _not_ be loaded when the simulator is run with a static reaction level of 4, regardless of the fact that they are in the appropriate difficulty `osm` file.

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTU0NTk3MDM0OSwxODUwNTY1OTU3LC0xMj
E0NzMwNTgzLC04Nzk1MzAzNDIsMTE2NTQyMjg4Niw2MzExNDE4
NzEsLTE3NzYyMjM3OTNdfQ==
-->