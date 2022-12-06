import 'package:flutter/material.dart';

class MyButton extends StatelessWidget {
  final String text;

  MyButton(this.text);

  @override
  Widget build(BuildContext context) {
    return Container(
      decoration: BoxDecoration(
          color: Theme.of(context).primaryColor,
          border: Border.all(
            color: Colors.yellowAccent,
            width: 2,
          ),
          borderRadius: BorderRadius.all(Radius.circular(20))),
      padding: EdgeInsets.all(16.0),
      child: Text(
        this.text,
        style: TextStyle(
            fontSize: 24, color: Colors.white, fontWeight: FontWeight.bold),
      ),
    );
  }
}
