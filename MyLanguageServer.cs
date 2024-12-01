using OmniSharp.Extensions.LanguageServer.Protocol.Models;
using OmniSharp.Extensions.LanguageServer.Protocol.Server.Capabilities;
using OmniSharp.Extensions.LanguageServer.Protocol;
using OmniSharp.Extensions.LanguageServer.Protocol.Server;
using OmniSharp.Extensions.LanguageServer.Server;

namespace Server;

/// <summary>
/// Contains custom LSP functionality:
/// 1. Initialization: Server capabilities
/// 2. Start: idk
/// 3. Stop: idk
/// </summary>
public static class MyLanguageServer
{
    public static Task InitializeAsync(ILanguageServer server, InitializeParams request, CancellationToken cancellationToken)
    {
        Console.WriteLine("Server initialized");
        return Task.CompletedTask;
    }

    public static Task StartAsync(ILanguageServer server, CancellationToken cancellationToken)
    {
        Console.WriteLine("Server started");
        return Task.CompletedTask;
    }
    
    public static Task ExitAsync(ExitParams exitParams, CancellationToken cancellationToken)
    {
        Console.WriteLine("Server stopped");
        return Task.CompletedTask;
    }
}