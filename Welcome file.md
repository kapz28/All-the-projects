# GeoScenario 1.1

GeoScenario is the format used to represent scenarios.
It is based on the OpenStreetMap format; every GeoScenario document is a valid `.osm` document, using custom properties to define scenario-specific information.

## Nodes

A _node_ is a single LLA point. GeoScenario defines the following presets for nodes:

### Ego Start

Use preset `ego_start.xml`.

Indicates the spawn location of the ego. Only one of these should exist per scenario. The ego will be spawned at the location of the first one, with the provided rotation.

- **Name** (`name`): The name given to this spawn location.
- **Orientation** (`orientation`): The heading that the ego should be spawned at, in degrees clockwise from East.
- **Speed**: _Currently not implemented_
- **Path ID**: _Currently not implemented_

### Ego Goal

Use preset `ego_goal.xml`.

Indicates a goal point for the ego. Multiple goals can exist per scenario. At runtime, a trigger with radius 2 m will be spawned at the goal point, and will end the scenario as soon as the ego reaches it.

- **Name** (`name`): The name given to this goal point.
- **Order** (`order`): Can be used as an unordered goal which is represented by only a value of 0 or can be used as an ordered goal which can exclusively one of any value {1,2,3,4,5,..}. 

Ordered goals are used when multiple goals must be reached in a certain order. Any unordered goal is interpreted as sufficient condition for termination of a scenario with success state. If order and unordered are available, a scenario must success if at least one unordered goal is reached, or all ordered goals are reached in the right order.

### Vehicle

Use preset `vehicle.xml`.

Indicates that a scenario vehicle should be spawned at the provided node.

- **Name** (`name`): The name assigned to this vehicle.
- **Dynamic Interaction Levels** (`dynamicinteractionlevels`): The Dynamic Interaction levels at which this vehicle will spawn. The provided value for this field must be a "list of ranges". The format for a list of ranges is indicated below.
- **Rotation** (`rotation`): The heading that the vehicle should be spawned at, in degrees clockwise from East.
- **Speed** (`speed`): The default speed that this vehicle will drive at, in km/h.
- **Use Speed Profile** (`usespeedprofile`): `true` if this vehicle should follow the speed profile of its specified path; `false` if it should drive at a constant default speed.
- **Path ID** (`path`): The name of the path that this vehicle is assigned to.
- **Start In Motion** (`startinmotion`): `true` if this vehicle should begin moving immediately when the scenario starts; `false` if it should remain stationary. This property will have no effect unless the vehicle has a path assigned.
- **Cycle Limit** (`cyclelimit`): The maximum number of times that a vehicle will complete its full path. If set to -1, the vehicle will repeat its path forever.
- **Loop** (`loop`): `true` if the vehicle should treat its assigned path as a closed loop. If `false`, the vehicle will teleport back to the first point once it reaches the last point, assuming that it has not yet reached its cycle limit. If `true`, the vehicle will drive from the last point back to the first point instead.
- **Driver**: _Currently not implemented_

### Pedestrian

Use preset `pedestrian.xml`.

Indicates that a scenario pedestrian should be spawned at the provided node.

- **Name** (`name`): The name assigned to this pedestrian.
- **Dynamic Interaction Levels** (`dynamicinteractionlevels`): The Dynamic Interaction levels at which this pedestrian will spawn. The provided value for this field must be a "list of ranges". The format for a list of ranges is indicated below.
- **Rotation** (`rotation`): The heading that the pedestrian should be spawned at, in degrees clockwise from East.
- **Speed** (`speed`): The default speed that this pedestrian will drive at, in km/h.
- **Use Speed Profile** (`usespeedprofile`): `true` if this pedestrian should follow the speed profile of its specified path; `false` if it should drive at a constant default speed.
- **Path ID** (`path`): The name of the path that this pedestrian is assigned to.
- **Start In Motion** (`startinmotion`): `true` if this pedestrian should begin moving immediately when the scenario starts; `false` if it should remain stationary. This property will have no effect unless the pedestrian has a path assigned.
- **Cycle Limit** (`cyclelimit`): The maximum number of times that a pedestrian will complete its full path. If set to -1, the pedestrian will repeat its path forever.
- **Loop** (`loop`): `true` if the pedestrian should treat its assigned path as a closed loop. If `false`, the pedestrian will teleport back to the first point once it reaches the last point, assuming that it has not yet reached its cycle limit. If `true`, the pedestrian will drive from the last point back to the first point instead.

### Static Object

Use preset `static_object.xml`.

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
- **Area** (`area`): `"yes"` this object must be interpreted as a polygon following the shape of the way forming an area. This is only true if the last and first nodes of the way are pointing to the same node. Ex: snow piles accumulating at the side of the road and blocking the drivable area., and `"no"` indicates the object boundaries for it's dimensions (maximum width and length). However, the object still keeps its natural shape. Example: a garbage bin. Note when object is defined as single node attribute is always `"no"`
- **Height** (`height`): The object's height in cm
- **Orientation** (`orientation`): Object's orientation in degrees

### Scenario Trigger

Use preset `trigger.xml`.

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

Use preset `velocity_params.xml`.

- **Target speed of agent (km/h)** (`agentspeed`) - the velocity that the agent should have when it reaches this path node.
- **Target acceleration of agent (g)** (`agentacceleration`) - the acceleration the agent should use to achieve the velocity at the next node.
- **Time to reach target acceleration of agent (s)** (`timetoacceleration`) - the time in which an agent should reach the given acceleration. The acceleration is linearly increased from the agent's previous acceleration to the target one over this period of time.


### Spawn

_Currently not implemented_

## Ways

A _way_ is an ordered collection of nodes, typically denoting a path, route, or boundary.
GeoScenario defines two presets for ways:

### Vehicle Path

Use preset `vehicle_path.xml`.

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

Use preset `pedestrian_path.xml`.

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
eyJoaXN0b3J5IjpbLTE3NzYyMjM3OTNdfQ==
-->