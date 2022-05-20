
# NetworkTimeSync

 

A simple C++ (and Blueprint compatible) plugin that aims to provide a more accurate server world time to clients.

## Installation

Simply place the contents of this repository in a `Plugins/NetworkTimeSync/` folder of your project.

You must then enable the plugin in your Project Settings, and in your `ProjectName.Build.cs` file.

```cs
PublicDependencyModuleNames.Add("NetworkTimeSync");
```

Once compiled and enabled, simply add the `UNetworkTimeSyncComponent` to your Player Controller. This component will be in charge of synchronizing network time, every 10 seconds by default.

![image](https://user-images.githubusercontent.com/25248023/169562656-6253804e-c176-496e-aa28-5ba9b9afb97e.png)


## Usage

The `NetworkTimeSubsystem` is a GameInstance subsystem, which will contain the latest server world time.
You can access it in C++ via the GameInstance, or by using the static provided:

```cpp
// via the GameInstance
if (UGameInstance* GI = GetWorld()->GetGameInstance())
{
  UNetworkTimeSubsystem* NetworkTime = GI->GetSubsystem<UNetworkTimeSubsystem>();
  const float ServerTime = NetworkTime->GetServerWorldTime();
}

// or, you can use the static with a world context object.
UNetworkTimeSubsystem* NetworkTime = UNetworkTimeSubsystem::Get(this);
// May return null with an invalid world context object.
if (NetworkTime)
{
  const float ServerTime = NetworkTime->GetServerWorldTime();
}
```

For Blueprints, you can simply access it via the generated node:

![image](https://user-images.githubusercontent.com/25248023/169562756-5a5710e7-eb52-40ee-a8cb-47fb732b52ab.png)

All you have to do is replace any calls to the GameState's `GetServerWorldTimeSeconds()` function with this one.

## Configuration

You can configure the synchronization interval in your Project Settings.

By default, it will reliably synchronize every ten seconds. You may want to increase or decrease that interval based on your preference.

> Warning: if you decrease the interval for it to synchronize very often, enable `Use Unreliable RPCs` to avoid saturating your bandwidth with RPCs.

![image](https://user-images.githubusercontent.com/25248023/169563307-fdc768b3-020f-4e75-9f29-3a9a753a5ca8.png)

