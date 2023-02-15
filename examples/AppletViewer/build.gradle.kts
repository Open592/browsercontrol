plugins {
    application
    base
    id("java")
}

group = "com.open592"
version = "1.0-SNAPSHOT"

application {
    applicationDefaultJvmArgs = listOf(
            "-Djava.library.path=${System.getenv("BC_OUTPUT_PATH")}",
            "-Dcom.open592.workingDirectory=${System.getenv("BC_OUTPUT_PATH")}"
    )

    mainClass.set("com.open592.appletviewer.AppletViewer")
}

repositories {
    mavenCentral()
}

dependencies {
    testImplementation("org.junit.jupiter:junit-jupiter-api:5.8.1")
    testRuntimeOnly("org.junit.jupiter:junit-jupiter-engine:5.8.1")
}

tasks.getByName<Test>("test") {
    useJUnitPlatform()
}
