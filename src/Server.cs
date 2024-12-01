using OmniSharp.Extensions.LanguageServer.Server;
using OmniSharp.Extensions.LanguageServer.Protocol.Server;
using System.Threading.Tasks;
using OmniSharp.Extensions.LanguageServer.Protocol.Document;
using OmniSharp.Extensions.LanguageServer.Protocol.Models;
using System.Threading;
using Microsoft.Extensions.DependencyInjection;
using System.Diagnostics.CodeAnalysis;
using OmniSharp.Extensions.LanguageServer.Protocol.General;
using System.Reflection;
using System.Runtime.CompilerServices;
using Server.handler;

namespace Server.src;

class Server
{
    [NotNull]
    public static LanguageServer? _server { get; private set; }

    /// <summary>
    /// Gets the directory, that the caller is in: if called from MAIN, returns the root dir
    /// </summary>
    private static string GetRootDir([CallerFilePath] string path = null!)
    {
        return Path.GetDirectoryName(path)!;
    }
    public static string ROOT_DIR { get; private set; } = GetRootDir();
    public static string LOG_DIR { get; private set; } = Path.Join(ROOT_DIR, "logs", "logDump.log");
    static async Task Main(string[] args)
    {
        StreamWriter logFile = new StreamWriter(LOG_DIR, true);
        logFile.AutoFlush = true;

        Logger logger = new Logger(Console.Out, logFile);

        Console.SetOut(logger);

        Console.WriteLine("TEST");

        _server = await LanguageServer.From(options =>
            options
                .WithInput(Console.OpenStandardInput())
                .WithOutput(Console.OpenStandardOutput())
                .WithHandler<TextDocumentHandler>()
                .OnInitialize(MyLanguageServer.InitializeAsync)
                .OnStarted(MyLanguageServer.StartAsync)
                .OnExit(MyLanguageServer.ExitAsync)
        );

        await _server.WaitForExit;
    }
}
