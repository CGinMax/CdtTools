import QtQuick 2.12
import QtQuick.Window 2.12
//import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4 as Old
import QtQuick.Layouts 1.12
import Qaterial 1.0 as Qaterial

import "gather"
import "sensor"

Qaterial.ApplicationWindow {
    id: main_window
    width: 960
    height: 720

    title: qsTr("Press Plate Tool")

    Old.SplitView {
        anchors.fill: parent
        GatherConfView{
            id: view_gather_config
            width: 300
        }

        StackLayout {
            currentIndex: view_gather_config.gather_count == 0 ? 0 : 1
            Rectangle {
            }

            SensorConfView{
            }
        }



        handleDelegate: Rectangle {
            width: 2
            color: "grey"

        }
    }

    Component.onCompleted: {
        Qaterial.Style.theme = Qaterial.Style.Theme.Light
        main_window.x = (Screen.width - width) / 2
        main_window.y = (Screen.height - height) / 2
        main_window.visible = true
    }



}