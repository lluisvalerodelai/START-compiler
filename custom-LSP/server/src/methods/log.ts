import { createWriteStream } from "fs";
import * as path from "path";
import { projectRoot } from "./projectRoot";

const log = createWriteStream(path.join(projectRoot, "log", "serverLog.log"));

export default {
    write : (message : object | unknown) => {
        if (typeof message === "object") {
            log.write(JSON.stringify(message));
        }
        else {
            log.write(message);
        }
        log.write("\n");
    }
}